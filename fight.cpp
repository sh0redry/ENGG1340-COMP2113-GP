#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>zz
#include <sys/ioctl.h>
#endif

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
auto startTime = chrono::steady_clock::now(); // 游戏开始时间
int gameDuration = 60; // 游戏持续时间（秒）
int fps=120;   //帧率，可以看作每一帧之间间隔的时间


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


// 控制台句柄
#ifdef _WIN32
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#else
struct termios originalTermios;
#endif

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
// MAC系统下的控制台输入输出设置
#ifndef _WIN32

int _kbhit() {
    int bytesWaiting;
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
    return bytesWaiting > 0;
}

int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
void setTerminalMode() {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &originalTermios);
    newt = originalTermios;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
void restoreTerminalMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
}

void InitConsole() {
    cout << "\033[?25l"; // 隐藏光标
    cout << "\033[2J\033[H"; // 清屏并移动光标到左上角
}
void RestoreConsole() {
    cout << "\033[?25h"; // 显示光标
}
void MoveCursor(short x, short y) {
    // ANSI escape sequence to move the cursor
    cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}
#else

void InitConsole()
{
    // 设置控制台缓冲区大小
    COORD bufferSize = { WIDTH + 2, HEIGHT + 3 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // 设置窗口大小
    SMALL_RECT windowRect = { 0, 0, WIDTH + 1, HEIGHT + 2 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowRect);

    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void RestoreConsole() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void MoveCursor(short x, short y) {
    SetConsoleCursorPosition(hConsole, { x, y });
}
#endif


void Draw()
{   
    // 获取控制台宽度
    int consoleWidth = 80; // 默认值为80 后面会更新
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    consoleWidth = w.ws_col;
#endif
    //计算所需的空格数
    int gameAreaWidth = WIDTH + 2;
    int padding = max(0, (consoleWidth - gameAreaWidth) / 2); 

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
    for (size_t i = 0; i < bullets.size(); ++i) { // 使用索引循环
        if (bullets[i].second >= 0 && bullets[i].second < HEIGHT)
            scene[bullets[i].second][bullets[i].first] = BULLET_CHAR;
    }

    // 放置敌人
    for (size_t i = 0; i < enemies.size(); ++i) { // 使用索引循环
        if (enemies[i].y >= 0 && enemies[i].y < HEIGHT)
            scene[enemies[i].y][enemies[i].x] = enemies[i].GetDisplayChar();
    }

    // 绘制场景
    MoveCursor(0, 0);
    
    // 绘制上边框
    cout << string(padding, ' ')<< "+"; 
    for (int i = 0; i < WIDTH; i++) cout << '-';
    cout << "+\n";

    // 绘制游戏区域
    for (int y = 0; y < HEIGHT; y++) {
        cout << string(padding, ' ') << '|' << scene[y] << "|\n";
    }

    // 得到剩余时间
    auto currentTime = chrono::steady_clock::now();
    int remainingTime = gameDuration - chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
    remainingTime = max(0, remainingTime);

    // 绘制下边框和血量
    cout << string(padding, ' ') << '+';
    for (int i = 0; i < WIDTH; i++) cout << '-';
    cout << "+\n";

    string homeLine = "YOUR HOME";
    int homePadding = (consoleWidth - homeLine.length()) / 2;
    cout << string(homePadding, ' ') << homeLine << endl;

    string statusLine = "HP: " + to_string(HP) + " | Time left: " + to_string(remainingTime);
    int statusPadding = (consoleWidth - statusLine.length()) / 2;
    cout << string(statusPadding, ' ') << statusLine << endl;   // 显示血量和剩余时间

    string hintLine = "Press A/D to move left/right, Space to shoot, Z/C to move faster.";
    int hintPadding = (consoleWidth - hintLine.length()) / 2;
    cout << string(hintPadding, ' ') << hintLine << endl; // 提示信息

    string weaponLine = "Weapon damage: " + to_string(BulletDamage) + " Enemy HP: " + to_string(enemyInitHP);
    int weaponPadding = (consoleWidth - weaponLine.length()) / 2;
    cout << string(weaponPadding, ' ') << weaponLine << endl; // 显示武器伤害和敌人血量
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
    for (size_t i = 0; i < bullets.size(); ++i) { // 使用索引循环
        bullets[i].second--;
    }
        bullets.erase(remove_if(bullets.begin(), bullets.end(), IsBulletOutOfRange()), bullets.end());

    

    // 移动敌人并计算逃脱的敌人
    int escaped = 0;
    for (size_t i = 0; i < enemies.size(); ++i) { // 使用索引循环
        if (random_range(0, 99) < enemySpeed) { // 敌人每帧向下移动的期望为enemySpeed
            enemies[i].y++;
        } 
        if (enemies[i].y >= HEIGHT) {
            escaped++;
        }
    }
    HP -= escaped * 10;
    if (HP < 0) HP = 0;
    enemies.erase(remove_if(enemies.begin(), enemies.end(), IsEnemyOutOfRange()), enemies.end());

    // 生成新敌人

    if (random_range(0, 99) < enemynum) {
        int x = random_range(0, WIDTH - 1);
        Enemy newEnemy;
        newEnemy.x = x;
        newEnemy.y = 0;
        newEnemy.health = enemyInitHP;
        enemies.push_back(newEnemy);
    }


    // 碰撞检测（子弹和敌人）
    for (auto b = bullets.begin(); b != bullets.end();) {
        bool hit = false;
        for (auto e = enemies.begin(); e != enemies.end();) {
            if (b->first == e->x && b->second == e->y) {
                e->health -= BulletDamage; // 减少敌人血量
                if (e->health <= 0) {
                    e = enemies.erase(e); // 删除被击毁敌人
                } else {
                    ++e;  // 保留受伤的敌人
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
        if (e.x == playerX && e.y == playerY) { // 玩家与敌人相撞,游戏结束
            HP -= 10; // 玩家血量减少
            e.health = 0; // 敌人被击毁
            if (HP <= 0) { // 玩家血量为0，游戏结束
                gameOver = true;
                return;
            }
        }
    }
    if (HP <= 0) { // 玩家血量为0，游戏结束
        gameOver = true;
        return;
    }
}
// 处理用户输入
void ProcessInput() {
#ifdef _WIN32
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0xE0) { // 处理方向键
            ch = _getch();
            switch (ch) {
                case 75: // 左键
                    if (playerX > 0) playerX--;
                    break;
                case 77: // 右键
                    if (playerX < WIDTH-1) playerX++;
                    break;
            }
        }
        else {
            switch (toupper(ch)) { // 新增AD键处理
                case 'A': // 左移
                    if (playerX > 0) playerX--;
                    break;
                case 'D': // 右移
                    if (playerX < WIDTH-1) playerX++;
                    break;
                case 32:  // 空格键射击
                    if (mutiple ==1 ){// 单发模式
                    bullets.emplace_back(playerX, playerY-1);
                    break;
                    } else if (mutiple == 3 && playerX > 1 && playerX < WIDTH-2) { // 三连发模式
                        bullets.emplace_back(playerX-1, playerY-1);
                        bullets.emplace_back(playerX, playerY-1);
                        bullets.emplace_back(playerX+1, playerY-1);
                        break;
                    } else if (mutiple == 5 && playerX > 2 && playerX < WIDTH-3) { // 五连发模式
                        bullets.emplace_back(playerX-2, playerY-1);
                        bullets.emplace_back(playerX-1, playerY-1);
                        bullets.emplace_back(playerX, playerY-1);
                        bullets.emplace_back(playerX+1, playerY-1);
                        bullets.emplace_back(playerX+2, playerY-1);
                        break;
                    } else if (mutiple ==5 && (playerX == 2 || playerX == WIDTH-3)) { // 边界情况
                        bullets.emplace_back(playerX-1, playerY-1);
                        bullets.emplace_back(playerX, playerY-1);
                        bullets.emplace_back(playerX+1, playerY-1);
                        break;
                    } else {
                        bullets.emplace_back(playerX, playerY-1); // 剩余情况全部处理为单发模式
                        break;
                    }
                case 'Z':
                    if (playerX > 2) playerX-=3;
                        break;
                case 'C':
                    if (playerX < WIDTH-3) playerX+=3;
                        break;
                

            }
        }
    }
#else
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0x1B) { // ESC或方向键
            if (_kbhit()) {
                ch = _getch();
                if (ch == '[') {   
                    ch = _getch();
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
                    bullets.emplace_back(playerX, playerY - 1);
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
#endif
}


int main(){ 
    initGameData(); // 初始化游戏数据
    
 #ifndef _WIN32
    setTerminalMode();   // 进入非规范模式
#endif
    InitConsole();
    
    auto lastTime = chrono::steady_clock::now();
    
    bool timeOut = false;
    
    while (!gameOver) {
        auto currentTime = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(currentTime - lastTime).count();
        
        // 检查游戏总时间,如果超过设定时间则结束游戏
        auto timeElapsed = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        if (timeElapsed >= gameDuration) {
            gameOver = true;
            timeOut = true;
        }
    
        if (elapsed > fps) // 控制帧率，fps为间隔时间
        {
            ProcessInput();
            Update();
            Draw();
            lastTime = currentTime;
        }
        
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // 显示游戏结束画面
    if (!timeOut) {
        MoveCursor(0, HEIGHT + 2);
        cout << "Game Over! You have been eaten by zoombies..." << endl;
    } else {
        MoveCursor(0, HEIGHT + 2);
        cout << "Time's up! You survived for this night"  << endl;
        cout << "Surviver left: " << HP/100 +1 << endl; // 显示剩余幸存者
    }
    
    // 恢复光标
    RestoreConsole();
#ifndef _WIN32
    restoreTerminalMode();  // 恢复终端模式
#endif
 
    return 0;
}
