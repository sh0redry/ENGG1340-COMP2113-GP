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
int enemyInitHP = 1; // 敌机初始血量
int BulletDamage = 1; // 子弹伤害
int enemynum=11; // 敌机生成概率（15-30)比较合理
int fps=200;   //帧率，可以看作游戏刷新率
int initialHP = 100; // 初始血量

//设置enemy结构体
struct Enemy {
    int x;
    int y;
    int health;
    char GetDisplayChar() const {  // 根据血量显示不同字符
        if (health >= 2) return '*';
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
vector<pair<int, int> > bullets;//子弹位置（x,y）
vector<Enemy> enemies;
bool gameOver = false;
int HP = initialHP; // 玩家血量

// 随机数生成器
std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> dist(0, 99);

int random_range(int min, int max) {
    return min + (rand() % (max - min + 1));
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
    cout << '+';
    for (int i = 0; i < WIDTH; i++) cout << '-';
    cout << "+\n";

    // 绘制游戏区域
    for (int y = 0; y < HEIGHT; y++) {
        cout << '|' << scene[y] << "|\n";
    }

    // 绘制下边框和分数
    cout << '+';
    for (int i = 0; i < WIDTH; i++) cout << '-';
    cout << "+\n";
    cout << "HP: " << HP <<endl; // 显示血量
    cout << "Press A/D to move left/right, Space to shoot, Z/C to move faster." << endl; // 提示信息
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

    

    // 逃脱的敌人
    int escaped = 0;
    for (size_t i = 0; i < enemies.size(); ++i) { // 使用索引循环
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
            HP -= 1; // 玩家血量减少
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
                    bullets.emplace_back(playerX, playerY-1);
                    break;
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
 #ifndef _WIN32
    setTerminalMode();   // 进入非规范模式
#endif
    InitConsole();
    
    auto lastTime = chrono::steady_clock::now();
    
    while (!gameOver) {
        auto currentTime = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(currentTime - lastTime).count();
        if (elapsed > fps) // 控制帧率
        {
            ProcessInput();
            Update();
            Draw();
            lastTime = currentTime;
        }
        
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // 显示游戏结束画面
    MoveCursor(0, HEIGHT + 2);
    cout << "Game Over! Your brain has been eaten by zoombies..." << endl;
    
    // 恢复光标
    RestoreConsole();
#ifndef _WIN32
    restoreTerminalMode();  // 恢复终端模式
#endif
 
    return 0;
}