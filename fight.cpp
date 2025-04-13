#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>

using namespace std;

// 游戏参数
const int WIDTH = 25;
const int HEIGHT = 25;
const char PLAYER_CHAR = 'A';
const char BULLET_CHAR = '|';
int enemyInitHP = 1; // 敌机初始血量
int BulletDamage = 1; // 子弹伤害
int enemynum=15; // 敌机生成概率（15-30)比较合理
int fps=250;   //帧率，可以看作游戏刷新率
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
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// 游戏状态
int playerX = WIDTH / 2;
int playerY = HEIGHT - 1;
vector<pair<int, int>> bullets;
vector<Enemy> enemies;
bool gameOver = false;
int HP = initialHP; // 玩家血量

// 随机数生成器
mt19937 rng(random_device{}());
uniform_int_distribution<int> dist(0, 99);

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

void MoveCursor(short x, short y)
{
    SetConsoleCursorPosition(hConsole, { x, y });
}

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
    for (auto& b : bullets) {
        if (b.second >= 0 && b.second < HEIGHT)
            scene[b.second][b.first] = BULLET_CHAR;
    }

    // 放置敌人
    for (auto& e : enemies) {
        if (e.y >= 0 && e.y < HEIGHT)
            scene[e.y][e.x] = e.GetDisplayChar();
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
}

void Update() {
    // 移动子弹
    for (auto& b : bullets) b.second--;
    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](auto& b) { return b.second < 0; }), bullets.end());

    // 移动敌人
    for (auto& e : enemies) e.y++;
    int escaped = count_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return e.y >= HEIGHT; });
    HP -= escaped;
    if (HP < 0) HP = 0;
    //移除过界敌人
    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](auto& e) { return e.y >= HEIGHT; }), enemies.end());

    // 生成新敌人
    if (dist(rng) < enemynum) { // 敌人生成概率
        uniform_int_distribution<int> xDist(0, WIDTH-1);
        enemies.push_back({xDist(rng), 0, enemyInitHP}); // 随机生成敌人
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
            gameOver = true;
            return;
        }
    }
    if (HP <= 0) { // 玩家血量为0，游戏结束
        gameOver = true;
        return;
    }
}

void ProcessInput() {
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
                    if (playerX > 1) playerX-=2;
                        break;
                case 'C':
                    if (playerX < WIDTH-2) playerX+=2;
                        break;
                

            }
        }
    }
}

int main()
{
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
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    return 0;
}