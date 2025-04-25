#include "Combat.h"
#include <algorithm>
#include <thread>
#include <iostream>

Combat::Combat(int weaponLevel, int difficulty, int gameLevel, int people)
    : weapon(weaponLevel), zombieManager(difficulty, gameLevel),
      playerX(WIDTH / 2), playerY(HEIGHT - 1),
      initialHP(people * 100), HP(people * 100) {
    
    // 设置游戏时长
    int gameDurationArr[5] = {40, 40, 50, 50, 60};
    gameDuration = gameDurationArr[gameLevel - 1];
    startTime = std::chrono::steady_clock::now();
}

bool Combat::run() {
    auto& terminal = Terminal::GetInstance();
    terminal.HideCursor();
    
    // 初始绘制
    UI::ShowInterface("empty.txt");
    draw();
    
    while (HP > 0 && !isTimeUp()) {
        processInput();
        update();
        draw();
        
        // 控制帧率
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    terminal.ShowCursor();
    return HP > 0;
}

void Combat::processInput() {
    auto& terminal = Terminal::GetInstance();
    if (terminal.CheckInput()) {
        int ch = terminal.GetKeyPress();
        if (ch == 0x1B) { // ESC或方向键
            if (terminal.CheckInput()) {
                ch = terminal.GetKeyPress();
                if (ch == '[') {   
                    ch = terminal.GetKeyPress();
                    switch (ch) {
                        case 75: // 左
                            playerX = std::max(0, playerX - 1);
                            break;
                        case 77: // 右
                            playerX = std::min(WIDTH - 1, playerX + 1);
                            break;
                    }
                }
            }
        } else {
            switch (toupper(ch)) {
                case 'A':
                    playerX = std::max(0, playerX - 1);
                    break;
                case 'D':
                    playerX = std::min(WIDTH - 1, playerX + 1);
                    break;
                case ' ':
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
                case 'Z':
                    playerX = std::max(0, playerX - 3);
                    break;
                case 'C':
                    playerX = std::min(WIDTH - 1, playerX + 3);
                    break;
            }
        }
    }
}

void Combat::update() {
    // 移动子弹
    for (auto& bullet : bullets) {
        bullet.second--;
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const std::pair<int, int>& bullet) { return bullet.second < 0; }), bullets.end());
    
    // 更新僵尸
    zombieManager.update();
    
    // 处理子弹碰撞
    for (auto it = bullets.begin(); it != bullets.end();) {
        int damage = zombieManager.processCollision(it->first, it->second, weapon.getDamage());
        if (damage > 0) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // 处理逃脱的僵尸
    int escapedDamage = zombieManager.getEscapedZombies() * 10;
    if (escapedDamage > 0) {
        HP = std::max(0, HP - escapedDamage);
    }
}

void Combat::draw() const {
    auto& terminal = Terminal::GetInstance();
    auto size = terminal.GetTerminalSize();
    
    // 计算游戏区域在方框中的位置
    int gameLeft = (size.width - WIDTH) / 2;
    int gameTop = (size.height - (HEIGHT + 8)) / 2 + 3;
    
    // 创建场景缓冲区
    char scene[HEIGHT][WIDTH + 1] = {};
    
    // 初始化场景
    for (int y = 0; y < HEIGHT; y++) {
        std::fill_n(scene[y], WIDTH, ' ');
        scene[y][WIDTH] = '\0';
    }
    
    // 放置玩家
    scene[playerY][playerX] = PLAYER_CHAR;
    
    // 放置子弹
    for (const auto& bullet : bullets) {
        if (bullet.second >= 0 && bullet.second < HEIGHT)
            scene[bullet.second][bullet.first] = BULLET_CHAR;
    }
    
    // 放置敌人
    for (const auto& zombie : zombieManager.getZombies()) {
        if (zombie.y >= 0 && zombie.y < HEIGHT)
            scene[zombie.y][zombie.x] = zombie.getDisplayChar();
    }
    
    // 绘制游戏区域边框
    terminal.MoveCursor(gameLeft - 1, gameTop - 1);
    std::cout << "+";
    for (int i = 0; i < WIDTH; i++) std::cout << "-";
    std::cout << "+";
    
    // 绘制游戏区域
    for (int y = 0; y < HEIGHT; y++) {
        terminal.MoveCursor(gameLeft - 1, gameTop + y);
        std::cout << "|";
        terminal.MoveCursor(gameLeft, gameTop + y);
        std::cout << scene[y];
        terminal.MoveCursor(gameLeft + WIDTH, gameTop + y);
        std::cout << "|";
    }
    
    // 绘制下边框
    terminal.MoveCursor(gameLeft - 1, gameTop + HEIGHT);
    std::cout << "+";
    for (int i = 0; i < WIDTH; i++) std::cout << "-";
    std::cout << "+";
    
    // 显示状态信息
    int statusY = gameTop + HEIGHT + 1;
    
    // 清除状态区域
    for (int i = 0; i < 6; i++) {
        terminal.MoveCursor(gameLeft, statusY + i);
        std::cout << std::string(WIDTH, ' ');
    }
    
    std::string statusLine = "=== GAME STATUS ===";
    terminal.MoveCursor(gameLeft + (WIDTH - statusLine.length()) / 2, statusY);
    std::cout << statusLine;
    
    // 显示HP和幸存者数量
    int survivors = (HP + 99) / 100; // 向上取整计算幸存者数量
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
    
    // 强制刷新输出
    std::cout.flush();
}

bool Combat::isTimeUp() const {
    auto elapsed = std::chrono::steady_clock::now() - startTime;
    return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() >= gameDuration;
}

int Combat::getRemainingTime() const {
    auto remaining = gameDuration - 
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startTime).count();
    return std::max(0, static_cast<int>(remaining));
} 