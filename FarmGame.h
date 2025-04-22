#ifndef FARM_GAME_H
#define FARM_GAME_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

class FarmGame {
private:
    int crops_storage;
    int gold_storage;
    int available_members;
    int difficulty;

    // Helper functions
    void previewAndRestore(const std::string& filename, int line_num, int col_pos, const std::string& text);
    void displayMultiLineInBox(const std::string& filename, int start_line, int col_pos, const std::vector<std::string>& lines);
    void displayArtFile(const std::string& filename);
    void clearScreen();

    // Game state structure
    struct GameState {
        int resource_storage;
        int member_available;
    };

public:
    // Constructor
    FarmGame(int initial_crops = 1000, int initial_gold = 500, 
             int initial_members = 5, int game_difficulty = 1);

    // Core game functions
    void growCrops();
    void mineGold();
    void recruitMembers();

    // Getters
    int getCropsStorage() const { return crops_storage; }
    int getGoldStorage() const { return gold_storage; }
    int getAvailableMembers() const { return available_members; }
    int getDifficulty() const { return difficulty; }

    // Display current game state
    void displayGameState() const;

    // Utility functions
    void waitForEnter(const std::string& message = "Press enter to continue...");
};

#endif // FARM_GAME_H 