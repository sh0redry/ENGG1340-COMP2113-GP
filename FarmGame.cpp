#include "FarmGame.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

FarmGame::FarmGame(int initial_crops, int initial_gold, 
                   int initial_members, int game_difficulty)
    : crops_storage(initial_crops), gold_storage(initial_gold),
      available_members(initial_members), difficulty(game_difficulty) {}

void FarmGame::previewAndRestore(const string& filename, int line_num, int col_pos, const string& text) {
    vector<string> original_lines;
    ifstream in_file(filename);
    string line;
    
    while (getline(in_file, line)) {
        original_lines.push_back(line);
    }
    in_file.close();

    vector<string> modified_lines = original_lines;
    
    if (line_num > modified_lines.size()) {
        modified_lines.resize(line_num, "");
    }

    string& target_line = modified_lines[line_num-1];
    if (col_pos-1 > target_line.length()) {
        target_line.resize(col_pos-1, ' ');
    }

    target_line.replace(col_pos-1, text.length(), text);

    for (const auto& l : modified_lines) {
        cout << l << "\n";
    }

    ofstream out_file(filename);
    for (const auto& l : original_lines) {
        out_file << l << "\n";
    }
    out_file.close();
}

void FarmGame::displayMultiLineInBox(const string& filename, int start_line, int col_pos, const vector<string>& lines) {
    vector<string> original_lines;
    ifstream in_file(filename);
    string line;
    
    while (getline(in_file, line)) {
        original_lines.push_back(line);
    }
    in_file.close();

    vector<string> modified_lines = original_lines;
    
    if (start_line + lines.size() > modified_lines.size()) {
        modified_lines.resize(start_line + lines.size(), "");
    }

    for (size_t i = 0; i < lines.size(); i++) {
        string& target_line = modified_lines[start_line-1 + i];
        if (col_pos-1 > target_line.length()) {
            target_line.resize(col_pos-1, ' ');
        }
        target_line.replace(col_pos-1, lines[i].length(), lines[i]);
    }

    for (const auto& l : modified_lines) {
        cout << l << "\n";
    }

    ofstream out_file(filename);
    for (const auto& l : original_lines) {
        out_file << l << "\n";
    }
    out_file.close();
}

void FarmGame::displayArtFile(const string& filename) {
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        cout << line << "\n";
    }
    file.close();
}

void FarmGame::clearScreen() {
    cout << "\033[2J\033[1;1H";
    cout.flush();
}

void FarmGame::waitForEnter(const string& message) {
    cout << message;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void FarmGame::displayGameState() const {
    cout << "=== 当前游戏状态 ===" << endl;
    cout << "作物存储: " << crops_storage << " kg" << endl;
    cout << "黄金存储: " << gold_storage << " gold" << endl;
    cout << "可用成员: " << available_members << " 人" << endl;
    cout << "难度等级: " << difficulty << endl;
}

void FarmGame::growCrops() {
    int work_member;
    clearScreen();
    displayArtFile("farm_art1.txt");
    waitForEnter();
    clearScreen();
    
    int crops_harvest = 10 + 10 * difficulty;

    while (true) {
        string prompt = "Assign farmers (0-" + to_string(available_members) + "): ";
        previewAndRestore("explore.txt", 18, 33, prompt);
        
        if (!(cin >> work_member)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            previewAndRestore("explore.txt", 18, 33, "Invalid input! Enter an integer.");
        } else if (work_member < 0 || work_member > available_members) {
            string error = "Must be between 0 and " + to_string(available_members) + "!";
            previewAndRestore("explore.txt", 18, 33, error);
        } else {
            break;
        }
    }

    crops_storage += crops_harvest * work_member;
    available_members -= work_member;

    clearScreen();
    displayArtFile("farm_art1.txt");
    waitForEnter();    
    
    clearScreen();
    displayArtFile("farm_art2.txt");
    waitForEnter();

    clearScreen();
    vector<string> result_lines = {
        to_string(work_member) + " farmers working",
        "Harvest: " + to_string(crops_harvest) + " kg per farmer",
        "Total harvested: " + to_string(work_member * crops_harvest) + " kg"
    };
    displayMultiLineInBox("explore.txt", 18, 33, result_lines);
    waitForEnter();
    clearScreen();
}

void FarmGame::mineGold() {
    int work_member;
    
    clearScreen();
    displayArtFile("mine_art1.txt");
    waitForEnter();
    clearScreen(); 
    clearScreen();
    
    int gold_yield = 10 + 10 * difficulty;

    while (true) {
        string prompt = "Assign miners (0-" + to_string(available_members) + "): ";
        previewAndRestore("explore.txt", 18, 33, prompt);
        
        if (!(cin >> work_member)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            previewAndRestore("explore.txt", 18, 33, "Invalid input! Enter an integer.");
        } else if (work_member < 0 || work_member > available_members) {
            string error = "Must be between 0 and " + to_string(available_members) + "!";
            previewAndRestore("explore.txt", 18, 33, error);
        } else {
            break;
        }
    }

    gold_storage += gold_yield * work_member;
    available_members -= work_member;

    clearScreen();
    displayArtFile("mine_art1.txt");
    waitForEnter();

    clearScreen();
    displayArtFile("mine_art2.txt");
    waitForEnter();

    clearScreen();
    vector<string> result_lines = {
        to_string(work_member) + " miners working",
        "Yield: " + to_string(gold_yield) + " gold per miner",
        "Total mined: " + to_string(work_member * gold_yield) + " gold"
    };
    displayMultiLineInBox("explore.txt", 18, 33, result_lines);
    waitForEnter();
    clearScreen();
}

void FarmGame::recruitMembers() {
    clearScreen();
    displayArtFile("recruit_art1.txt");
    waitForEnter();
    clearScreen();

    const int base_cost = 100;
    const int cost_per_member = 100;
    
    int max_possible = (crops_storage - base_cost) / cost_per_member;
    max_possible = max(0, max_possible);

    vector<string> result_lines;
    int recruit_num = 0;

    if (max_possible <= 0) {
        int required = base_cost + cost_per_member;
        result_lines = {
            "CANNOT RECRUIT!",
            "Minimum required: " + to_string(required) + " crops",
            "You have: " + to_string(crops_storage) + " crops"
        };
        displayMultiLineInBox("explore.txt", 18, 33, result_lines);
        waitForEnter();
        return;
    }

    while (true) {
        string prompt = "Recruit members (0-" + to_string(max_possible) + " available): ";
        previewAndRestore("explore.txt", 18, 33, prompt);
        
        if (!(cin >> recruit_num)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            previewAndRestore("explore.txt", 18, 33, "Invalid input! Enter an integer.");
        } 
        else if (recruit_num < 0 || recruit_num > max_possible) {
            string error = "Must be between 0 and " + to_string(max_possible) + "!";
            previewAndRestore("explore.txt", 18, 33, error);
        } 
        else {
            break;
        }
    }

    if (recruit_num == 0) {
        result_lines = {"Recruitment canceled."};
        displayMultiLineInBox("explore.txt", 18, 33, result_lines);
        waitForEnter();
        return;
    }

    waitForEnter();
    clearScreen();
    displayArtFile("recruit_art2.txt");
    waitForEnter();
    clearScreen();

    int total_cost = base_cost + (cost_per_member * recruit_num);
    result_lines = {
        "SUCCESSFUL RECRUITMENT!",
        "New members: " + to_string(recruit_num),
        "Per member: " + to_string(cost_per_member) + " crops",
        "Total cost: " + to_string(total_cost) + " crops",
        "Remaining crops: " + to_string(crops_storage - total_cost)
    };
    
    displayMultiLineInBox("explore.txt", 18, 33, result_lines);
    waitForEnter();
    clearScreen();

    crops_storage -= total_cost;
    available_members += recruit_num;
} 