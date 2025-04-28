#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>
#include <termios.h>
#include <unistd.h>
using namespace std;

#define CLEAR_SCREEN "\033[2J\033[1;1H"

struct TerminalSettings {
    struct termios original;

    TerminalSettings() {
        // save original terminal settings
        tcgetattr(STDIN_FILENO, &original);
    }

    ~TerminalSettings() {
        // resume original settings when exit
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
    }
};

int extract_number(string& filename) {
    // delete .txt
    size_t dot_pos = filename.find(".txt");
    if (dot_pos == string::npos) return -1;

    // change int to string
    string num_str = filename.substr(0, dot_pos);
    try {
        return stoi(num_str);
    } catch (...) {
        return -1;
    }
}

// compare in descending order
bool compare_files(string& a, string& b) {
    return extract_number(a) > extract_number(b);
}

// get files
vector<string> get_files(string& dir_path) {
    vector<string> files;
    DIR* dir = opendir(dir_path.c_str());
    if (!dir) {
        cerr << "Error: Cannot open directory " << dir_path << endl;
        return files;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // pass hidden directory
        if (entry->d_name[0] == '.') continue;
        string filename = entry->d_name;
        // test filename format
        if (filename.find(".txt") != filename.length() - 4) continue;

        files.push_back(filename);
    }
    closedir(dir);
    return files;
}

void print_Ani(string target_dir, int time){
     vector<string> files = get_files(target_dir);

     if (files.empty()) {
         cerr << "No files found in " << target_dir << endl;
         exit(1);
     }

     sort(files.begin(), files.end(), compare_files);

     // output files
     for (const auto& filename : files) {
         cout << CLEAR_SCREEN;
         string full_path = target_dir + "/" + filename;
         ifstream fin;
         fin.open(full_path);
         if (fin.fail()){
           exit(1);
         }
         string str;
         while (getline(fin, str)){
           cout << str << endl;
         }
         this_thread::sleep_for(chrono::milliseconds(time));
     }
}

void enable_raw_mode() {
    struct termios raw = TerminalSettings().original;
    raw.c_lflag &= ~(ICANON | ECHO);  // close echo and cooked mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

//press any key to continue
void wait_any_key() {
    char buf[1];
    while(true) {
        if(read(STDIN_FILENO, buf, sizeof(buf)) > 0) break;
    }
}

int main() {
    if (true){ //lose
      //set path way
      string target_dir = "anim/Lose1";
      //print 'lose'
      print_Ani(target_dir, 50);

      TerminalSettings _;
      enable_raw_mode();
      wait_any_key();

      target_dir = "anim/Lose2";
      print_Ani(target_dir, 100);
    }
    else{ 
      string target_dir = "anim/Win1";
      print_Ani(target_dir, 80);

      TerminalSettings _;
      enable_raw_mode();
      wait_any_key();
      

      target_dir = "anim/Win3";
      print_Ani(target_dir, 80);
    }

    return 0;
}
