/**
 * @file Combat.cpp
 * @brief Implementation of the combat system
 * @details Manages the real-time combat mechanics including player movement,
 *          shooting, zombie interactions, collision detection, and game
 *          state management during combat sequences.
 */

#include "Combat.h"
#include <algorithm>
#include <thread>
#include <iostream>

// Initialize static member for pause functionality
Combat* Combat::currentInstance = nullptr;

/**
 * @brief Constructor for the Combat system
 * @param player Reference to the player object
 * @param weekCycle Reference to the week cycle object for difficulty scaling
 */
Combat::Combat(Player& player, const WeekCycle& weekCycle)
    : player(player),
      weapon(player.getWeaponLevel()), 
      zombieManager(player.getDifficulty(), weekCycle.getCurrentWeek()),
      playerX(WIDTH / 2), 
      playerY(HEIGHT - 1),
      initialHP(player.getTotalHP()), 
      HP(player.getTotalHP()),
      isPaused(false),
      pausedDuration(std::chrono::steady_clock::duration::zero()) {
    
    currentInstance = this;
    setupPauseCallback();
    
    // Set game duration based on current week
    int gameDurationArr[5] = {40, 40, 50, 50, 60};
    gameDuration = gameDurationArr[weekCycle.getCurrentWeek() - 1];
    startTime = std::chrono::steady_clock::now();
}

/**
 * @brief Sets up the pause callback function for the 'P' key
 */
void Combat::setupPauseCallback() {
    Terminal::GetInstance().SetPKeyCallback(togglePauseCallback);
}

/**
 * @brief Clears the pause callback function
 */
void Combat::clearPauseCallback() {
    Terminal::GetInstance().ClearPKeyCallback();
}

/**
 * @brief Static callback function to toggle pause state
 */
void Combat::togglePauseCallback() {
    if (currentInstance) {
        currentInstance->togglePause();
    }
}

/**
 * @brief Toggles the pause state of the game
 * Handles pause screen display and pause duration tracking
 */
void Combat::togglePause() {
    if (!isPaused) {
        // Enter pause state
        isPaused = true;
        pauseStartTime = std::chrono::steady_clock::now();
        if (SpecialFunctions::showPauseScreen()) {
            // If user presses P to continue, calculate pause duration and resume
            pausedDuration += std::chrono::steady_clock::now() - pauseStartTime;
            isPaused = false;
        }
    } else {
        // Resume game
        isPaused = false;
        pausedDuration += std::chrono::steady_clock::now() - pauseStartTime;
    }
}

/**
 * @brief Main game loop
 * Handles game initialization, main loop, and victory/defeat conditions
 * @return true if player wins, false if player loses
 */
bool Combat::run() {
    auto& terminal = Terminal::GetInstance();
    terminal.HideCursor();
    
    // Initial draw
    UI::ShowInterface("ui/empty.txt");
    draw();
    
    while (HP > 0 && !isTimeUp()) {
        if (isPaused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        processInput();
        update();
        draw();
        
        // Control frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    terminal.ShowCursor();
    clearPauseCallback();
    
    // Handle combat result
    bool victory = HP > 0;
    
    if (victory) {
        Animation::PlaySequence("anim/Win1", 80);
        UI::WaitForEnter("Press Enter to return to home...");
        Animation::PlaySequence("anim/Win2", 80);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } else {
        Animation::PlaySequence("anim/Lose1", 50);
        UI::WaitForEnter("Press Enter to quit...");
        Animation::PlaySequence("anim/Lose2", 100);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return victory;
}

/**
 * @brief Processes user input for player movement and shooting
 * Handles arrow keys, WASD, and space bar for shooting
 * Supports multiple shot patterns based on weapon level
 */
void Combat::processInput() {
    auto& terminal = Terminal::GetInstance();
    if (terminal.CheckInput()) {
        int ch = terminal.GetKeyPress();
        if (ch == 0x1B) { // ESC or arrow keys
            if (terminal.CheckInput()) {
                ch = terminal.GetKeyPress();
                if (ch == '[') {   
                    ch = terminal.GetKeyPress();
                    switch (ch) {
                        case 75: // Left arrow
                            playerX = std::max(0, playerX - 1);
                            break;
                        case 77: // Right arrow
                            playerX = std::min(WIDTH - 1, playerX + 1);
                            break;
                    }
                }
            }
        } else {
            switch (toupper(ch)) {
                case 'A': // Move left
                    playerX = std::max(0, playerX - 1);
                    break;
                case 'D': // Move right
                    playerX = std::min(WIDTH - 1, playerX + 1);
                    break;
                case ' ': // Shoot
                    if (weapon.getMultiple() == 1) {
                        bullets.emplace_back(playerX, playerY - 1);
                    } else if (weapon.getMultiple() == 3 && playerX > 1 && playerX < WIDTH - 2) {
                        bullets.emplace_back(playerX - 1, playerY - 1);
                        bullets.emplace_back(playerX, playerY - 1);
                        bullets.emplace_back(playerX + 1, playerY - 1);
                    } else if (weapon.getMultiple() == 5 && playerX > 2 && playerX < WIDTH - 3) {
                        bullets.emplace_back(playerX - 2, playerY - 1);
                        bullets.emplace_back(playerX - 1, playerY - 1);
                        bullets.emplace_back(playerX, playerY - 1);
                        bullets.emplace_back(playerX + 1, playerY - 1);
                        bullets.emplace_back(playerX + 2, playerY - 1);
                    } else if (weapon.getMultiple() == 5 && (playerX == 2 || playerX == WIDTH - 3)) {
                        bullets.emplace_back(playerX - 1, playerY - 1);
                        bullets.emplace_back(playerX, playerY - 1);
                        bullets.emplace_back(playerX + 1, playerY - 1);
                    } else {
                        bullets.emplace_back(playerX, playerY - 1);
                    }
                    break;
                case 'Z': // Quick move left
                    playerX = std::max(0, playerX - 3);
                    break;
                case 'C': // Quick move right
                    playerX = std::min(WIDTH - 1, playerX + 3);
                    break;
            }
        }
    }
}

/**
 * @brief Updates game state including bullets, zombies, and collisions
 * Handles bullet movement, zombie updates, and collision detection
 */
void Combat::update() {
    // Move bullets upward
    for (auto& bullet : bullets) {
        bullet.second--;
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const std::pair<int, int>& bullet) { return bullet.second < 0; }), bullets.end());
    
    // Update zombies
    zombieManager.update();
    
    // Process bullet collisions
    for (auto it = bullets.begin(); it != bullets.end();) {
        int damage = zombieManager.processCollision(it->first, it->second, weapon.getDamage());
        if (damage > 0) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // Process escaped zombies
    int escapedDamage = zombieManager.getEscapedZombies() * 10;
    if (escapedDamage > 0) {
        HP = std::max(0, HP - escapedDamage);
    }
}

/**
 * @brief Renders the current game state to the screen
 * Draws the game board, player, bullets, zombies, and status information
 */
void Combat::draw() const {
    auto& terminal = Terminal::GetInstance();
    auto size = terminal.GetTerminalSize();
    
    // Calculate game area position in the frame
    int gameLeft = (size.width - WIDTH) / 2;
    int gameTop = (size.height - (HEIGHT + 8)) / 2 + 3;
    
    // Create scene buffer
    char scene[HEIGHT][WIDTH + 1] = {};
    
    // Initialize scene
    for (int y = 0; y < HEIGHT; y++) {
        std::fill_n(scene[y], WIDTH, ' ');
        scene[y][WIDTH] = '\0';
    }
    
    // Place player
    scene[playerY][playerX] = PLAYER_CHAR;
    
    // Place bullets
    for (const auto& bullet : bullets) {
        if (bullet.second >= 0 && bullet.second < HEIGHT)
            scene[bullet.second][bullet.first] = BULLET_CHAR;
    }
    
    // Place enemies
    for (const auto& zombie : zombieManager.getZombies()) {
        if (zombie.y >= 0 && zombie.y < HEIGHT)
            scene[zombie.y][zombie.x] = zombie.getDisplayChar();
    }
    
    // Draw game area border
    terminal.MoveCursor(gameLeft - 1, gameTop - 1);
    std::cout << "+";
    for (int i = 0; i < WIDTH; i++) std::cout << "-";
    std::cout << "+";
    
    // Draw game area
    for (int y = 0; y < HEIGHT; y++) {
        terminal.MoveCursor(gameLeft - 1, gameTop + y);
        std::cout << "|";
        terminal.MoveCursor(gameLeft, gameTop + y);
        std::cout << scene[y];
        terminal.MoveCursor(gameLeft + WIDTH, gameTop + y);
        std::cout << "|";
    }
    
    // Draw bottom border
    terminal.MoveCursor(gameLeft - 1, gameTop + HEIGHT);
    std::cout << "+";
    for (int i = 0; i < WIDTH; i++) std::cout << "-";
    std::cout << "+";
    
    // Display status information
    int statusY = gameTop + HEIGHT + 1;
    
    // Clear status area
    for (int i = 0; i < 6; i++) {
        terminal.MoveCursor(gameLeft, statusY + i);
        std::cout << std::string(WIDTH, ' ');
    }
    
    std::string statusLine = "=== YOUR HOME ===";
    terminal.MoveCursor(gameLeft + (WIDTH - statusLine.length()) / 2, statusY);
    std::cout << statusLine;
    
    // Display HP and survivor count
    int survivors = (HP + 99) / 100; // Round up to calculate survivor count
    std::string hpLine = "HP: " + std::to_string(HP) + "/" + std::to_string(initialHP) + 
                        " (Survivors: " + std::to_string(survivors) + ")";
    terminal.MoveCursor(gameLeft + (WIDTH - hpLine.length()) / 2, statusY + 1);
    std::cout << hpLine;
    
    std::string timeLine = "Time left: " + std::to_string(getRemainingTime()) + "s";
    terminal.MoveCursor(gameLeft + (WIDTH - timeLine.length()) / 2, statusY + 2);
    std::cout << timeLine;
    
    std::string weaponLine = "Weapon: Lv." + std::to_string(weapon.getLevel()) + 
                           " (Damage: " + std::to_string(weapon.getDamage()) + ")";
    terminal.MoveCursor(gameLeft + (WIDTH - weaponLine.length()) / 2, statusY + 3);
    std::cout << weaponLine;
    
    std::string enemyLine = "Enemy HP: " + std::to_string(zombieManager.getInitHP());
    terminal.MoveCursor(gameLeft + (WIDTH - enemyLine.length()) / 2, statusY + 4);
    std::cout << enemyLine;
    
    std::string hintLine = "A/D: move left/right | Space: shoot | Z/C: move faster | P: pause";
    terminal.MoveCursor(gameLeft + (WIDTH - hintLine.length()) / 2, statusY + 5);
    std::cout << hintLine;

    // Force output flush
    std::cout.flush();
}

/**
 * @brief Checks if the game time has expired
 * @return true if time is up, false otherwise
 */
bool Combat::isTimeUp() const {
    auto elapsed = std::chrono::steady_clock::now() - startTime - pausedDuration;
    return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() >= gameDuration;
}

/**
 * @brief Calculates remaining game time
 * @return Remaining time in seconds
 */
int Combat::getRemainingTime() const {
    auto remaining = gameDuration - 
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startTime - pausedDuration).count();
    return std::max(0, static_cast<int>(remaining));
} 