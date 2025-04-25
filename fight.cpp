#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include "Terminal.h"
#include "UI.h"

using namespace std;

// 游戏参数
const int WIDTH = 25;
const int HEIGHT = 25;
const char PLAYER_CHAR = 'A';
const char BULLET_CHAR = '|';
const int DIFFICULTY = 1; // 难度系数，1-3, 从外界传入
const int GAME_LEVEL = 1; // 游戏等级，1-5 , 从外界传入
const int PEOPLE = 5; // 玩家数量，1-5, 从外界传入

// 武器参数
int weaponLevel = 3; // 武器等级，从外界传入
int weaponDamage [10] = {20 ,30, 40, 30, 35, 40, 50, 40, 50, 60 };
int BulletDamage = weaponDamage[weaponLevel-1]; // 子弹伤害
int weaponMutiple [10] = {1,1,1,3,3,3,3,5,5,5}; // 武器联装数量
int mutiple = weaponMutiple[weaponLevel-1]; // 武器倍数

//游戏难度参数
int enemyInitHP; // 敌机初始血量,从外界传入
int enemynum = 8; // 敌机生成概率
int initialHP = PEOPLE * 100; // 初始血量，为people * 100
int enemySpeed = 20; // 敌机速度,越小敌机越慢，必须＞0
int enemyMoveCounter = 0; // 敌机移动计数器
int enemySpawnCounter = 0; // 敌人生成计数器
int enemySpawnInterval = 30; // 敌人生成间隔（帧数）
auto startTime = chrono::steady_clock::now(); // 游戏开始时间
int gameDuration = 60; // 游戏持续时间（秒）
int fps=120;   //帧率，约60fps (1000ms/16ms ≈ 60fps)

const int TARGET_FPS = 60;  // 目标帧率
const int FRAME_TIME_MS = 1000 / TARGET_FPS;  // 每帧的目标时间（毫秒）

void initGameData(){
    if (DIFFICULTY == 1) { // 难度系数为1
        int enemyHParr [5]= {36, 45, 54, 63, 72}; // 难度为1时的血量
        enemyInitHP = enemyHParr[GAME_LEVEL-1]; // 敌机初始血量
        
    } else if (DIFFICULTY == 2) { 
        int enemyHParr [5]= {40, 50, 60, 70, 80}; // 难度为2时的血量
        enemyInitHP = enemyHParr[GAME_LEVEL-1]; // 敌机初始血量

    } else if (DIFFICULTY == 3) { // 难度系数为3
        int enemyHParr [5]= {44, 55, 66, 77, 88}; // 难度为3时的血量
        enemyInitHP = enemyHParr[GAME_LEVEL-1]; // 敌机初始血量
    }
    int gameDurationArr [5] = {40, 40, 50, 50, 60}; // 游戏时间
    gameDuration= gameDurationArr[GAME_LEVEL-1]; // 游戏时间
}
     
//设置enemy结构体
struct Enemy {
    int x;
    int y;
    int health;
    char GetDisplayChar() const {  
        if (health >= 45) return '*'; // 标记高血量敌人
        return '+';
    }
};

// 游戏状态
int playerX = WIDTH / 2;
int playerY = HEIGHT - 1;
vector<pair<int, int> > bullets;  //子弹位置（x,y）
vector<Enemy> enemies;
bool gameOver = false;
int HP = initialHP; // 初始化玩家血量

// 随机数生成器函数
int random_range(int min, int max) {
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

int _kbhit() {
    int bytesWaiting;
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
    return bytesWaiting > 0;
}

void Draw()
{   
    auto& terminal = Terminal::GetInstance();
    auto size = terminal.GetTerminalSize();
    
    // 只在游戏开始时显示方框
    static bool firstDraw = true;
    if (firstDraw) {
        UI::ShowInterface("empty.txt");
        firstDraw = false;
    }
    
    // 计算游戏区域在方框中的位置（水平和垂直都居中）
    int gameLeft = (size.width - WIDTH) / 2;
    int gameTop = (size.height - (HEIGHT + 8)) / 2 + 3;  // 总高度包括游戏区域和状态信息，+3是为了与顶部保持距离
    
    // 创建场景缓冲区
    char scene[HEIGHT][WIDTH + 1] = {};

    // 初始化场景
    for (int y = 0; y < HEIGHT; y++) {
        fill_n(scene[y], WIDTH, ' ');
        scene[y][WIDTH] = '\0';
    }

    // 放置玩家
    scene[playerY][playerX] = PLAYER_CHAR;

    // 放置子弹
    for (size_t i = 0; i < bullets.size(); ++i) {
        if (bullets[i].second >= 0 && bullets[i].second < HEIGHT)
            scene[bullets[i].second][bullets[i].first] = BULLET_CHAR;
    }

    // 放置敌人
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].y >= 0 && enemies[i].y < HEIGHT)
            scene[enemies[i].y][enemies[i].x] = enemies[i].GetDisplayChar();
    }

    // 绘制游戏区域边框
    terminal.MoveCursor(gameLeft - 1, gameTop - 1);
    cout << "+";
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << "+";

    // 绘制游戏区域
    for (int y = 0; y < HEIGHT; y++) {
        terminal.MoveCursor(gameLeft - 1, gameTop + y);
        cout << "|";
        terminal.MoveCursor(gameLeft, gameTop + y);
        cout << scene[y];
        terminal.MoveCursor(gameLeft + WIDTH, gameTop + y);
        cout << "|";
    }

    // 绘制下边框
    terminal.MoveCursor(gameLeft - 1, gameTop + HEIGHT);
    cout << "+";
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << "+";

    // 得到剩余时间
    auto currentTime = chrono::steady_clock::now();
    int remainingTime = gameDuration - chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
    remainingTime = max(0, remainingTime);

    // 显示状态信息
    int statusY = gameTop + HEIGHT + 1;
    
    // 清除状态区域
    for (int i = 0; i < 6; i++) {
        terminal.MoveCursor(gameLeft, statusY + i);
        cout << string(WIDTH, ' ');
    }
    
    string statusLine = "=== GAME STATUS ===";
    terminal.MoveCursor(gameLeft + (WIDTH - statusLine.length()) / 2, statusY);
    cout << statusLine;

    string hpLine = "HP: " + to_string(HP) + "/" + to_string(initialHP);
    terminal.MoveCursor(gameLeft + (WIDTH - hpLine.length()) / 2, statusY + 1);
    cout << hpLine;

    string timeLine = "Time left: " + to_string(remainingTime) + "s";
    terminal.MoveCursor(gameLeft + (WIDTH - timeLine.length()) / 2, statusY + 2);
    cout << timeLine;

    string weaponLine = "Weapon: Lv." + to_string(weaponLevel) + " (Damage: " + to_string(BulletDamage) + ")";
    terminal.MoveCursor(gameLeft + (WIDTH - weaponLine.length()) / 2, statusY + 3);
    cout << weaponLine;

    string enemyLine = "Enemy HP: " + to_string(enemyInitHP);
    terminal.MoveCursor(gameLeft + (WIDTH - enemyLine.length()) / 2, statusY + 4);
    cout << enemyLine;

    // 强制刷新输出
    cout.flush();
}

// 移除子弹的谓词函数
struct IsBulletOutOfRange {
    bool operator()(const pair<int, int>& bullet) {
        return bullet.second < 0;
    }
};

// 移除敌人的谓词函数
struct IsEnemyOutOfRange {
    bool operator()(const Enemy& enemy) {
        return enemy.y >= HEIGHT;
    }
};

//更新游戏状态
void Update() {
    // 移动子弹
    for (size_t i = 0; i < bullets.size(); ++i) {
        bullets[i].second--;
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(), IsBulletOutOfRange()), bullets.end());

    // 移动敌人并计算逃脱的敌人
    int escaped = 0;
    enemyMoveCounter++;
    if (enemyMoveCounter >= enemySpeed) {  // 只有当计数器达到一定值时敌人才移动
        enemyMoveCounter = 0;
        for (size_t i = 0; i < enemies.size(); ++i) {
            enemies[i].y++;
            if (enemies[i].y >= HEIGHT) {
                escaped++;
            }
        }
    }
    HP -= escaped * 10;
    if (HP < 0) HP = 0;
    enemies.erase(remove_if(enemies.begin(), enemies.end(), IsEnemyOutOfRange()), enemies.end());

    // 生成新敌人
    enemySpawnCounter++;
    if (enemySpawnCounter >= enemySpawnInterval) {
        enemySpawnCounter = 0;
        if (random_range(0, 99) < enemynum) {
            int x = random_range(0, WIDTH - 1);
            Enemy newEnemy;
            newEnemy.x = x;
            newEnemy.y = 0;
            newEnemy.health = enemyInitHP;
            enemies.push_back(newEnemy);
        }
    }

    // 碰撞检测（子弹和敌人）
    for (auto b = bullets.begin(); b != bullets.end();) {
        bool hit = false;
        for (auto e = enemies.begin(); e != enemies.end();) {
            if (b->first == e->x && b->second == e->y) {
                e->health -= BulletDamage;
                if (e->health <= 0) {
                    e = enemies.erase(e);
                } else {
                    ++e;
                }
                hit = true;
                break;
            } else {
                ++e;
            }
        }
        if (hit) {
            b = bullets.erase(b);
        } else {
            ++b;
        }
    }

    // 碰撞检测（玩家和敌人）
    for (auto& e : enemies) {
        if (e.x == playerX && e.y == playerY) {
            HP -= 10;
            e.health = 0;
            if (HP <= 0) {
                gameOver = true;
                return;
            }
        }
    }
    if (HP <= 0) {
        gameOver = true;
        return;
    }
}

// 处理用户输入
void ProcessInput() {
    auto& terminal = Terminal::GetInstance();
    if (_kbhit()) {
        int ch = terminal.GetKeyPress();
        if (ch == 0x1B) { // ESC或方向键
            if (_kbhit()) {
                ch = terminal.GetKeyPress();
                if (ch == '[') {   
                    ch = terminal.GetKeyPress();
                    switch (ch) {
                        case 75: // 左
                            playerX = max(0, playerX - 1);
                            break;
                        case 77: // 右
                            playerX = min(WIDTH - 1, playerX + 1);
                         break;
                    }
                }
            }
        } else {
            switch (toupper(ch)) {
                case 'A':
                    playerX = max(0, playerX - 1);
                    break;
                case 'D':
                    playerX = min(WIDTH - 1, playerX + 1);
                    break;
                case ' ':
                    if (mutiple == 1) {
                        bullets.emplace_back(playerX, playerY-1);
                    } else if (mutiple == 3 && playerX > 1 && playerX < WIDTH-2) {
                        bullets.emplace_back(playerX-1, playerY-1);
                        bullets.emplace_back(playerX, playerY-1);
                        bullets.emplace_back(playerX+1, playerY-1);
                    } else if (mutiple == 5 && playerX > 2 && playerX < WIDTH-3) {
                        bullets.emplace_back(playerX-2, playerY-1);
                        bullets.emplace_back(playerX-1, playerY-1);
                        bullets.emplace_back(playerX, playerY-1);
                        bullets.emplace_back(playerX+1, playerY-1);
                        bullets.emplace_back(playerX+2, playerY-1);
                    } else if (mutiple == 5 && (playerX == 2 || playerX == WIDTH-3)) {
                        bullets.emplace_back(playerX-1, playerY-1);
                        bullets.emplace_back(playerX, playerY-1);
                        bullets.emplace_back(playerX+1, playerY-1);
                    } else {
                        bullets.emplace_back(playerX, playerY-1);
                    }
                    break;
                case 'Z':
                    playerX = max(0, playerX - 3);
                    break;
                case 'C':
                    playerX = min(WIDTH - 1, playerX + 3);
                    break;
            }
        }
    }
}

int main(){
    initGameData();
    
    auto& terminal = Terminal::GetInstance();
    terminal.HideCursor();
    
    auto lastTime = chrono::steady_clock::now();
    bool timeOut = false;
    
    // 初始绘制
    Draw();
    
    while (!gameOver) {
        auto currentTime = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(currentTime - lastTime).count();
        
        // 检查游戏总时间
        auto timeElapsed = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        if (timeElapsed >= gameDuration) {
            gameOver = true;
            timeOut = true;
        }
    
        // 如果已经过了足够的时间，更新游戏状态
        if (elapsed >= FRAME_TIME_MS) {
            ProcessInput();
            Update();
            Draw();
            lastTime = currentTime;
            
            // 计算需要sleep的时间，以保持稳定的帧率
            auto frameEndTime = chrono::steady_clock::now();
            auto frameDuration = chrono::duration_cast<chrono::milliseconds>(frameEndTime - currentTime).count();
            if (frameDuration < FRAME_TIME_MS) {
                this_thread::sleep_for(chrono::milliseconds(FRAME_TIME_MS - frameDuration));
            }
        }
    }

    // 显示游戏结束画面
    terminal.Clear();
    // if (!timeOut) {
    //     UI::ShowInterface("game_over.txt");
    // } else {
    //     UI::ShowInterface("time_up.txt");
    //     terminal.MoveCursor(15, 15);
    //     cout << "Surviver left: " << HP/100 +1 << endl;
    // }
    
    terminal.ShowCursor();
    return 0;
}
