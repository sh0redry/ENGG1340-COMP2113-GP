void counterWeapon(int &gold, int &weaponLevel, int &BulletDamage, int &multiple) {
    int damageForm[10] = {20, 30, 40, 25, 30, 40, 30, 35, 40, 50};
    int linesForm[10] = {1, 1, 1, 3, 3, 3, 3, 5, 5, 5};

    int goldNeed;
    string weapon;

    while (true) {
        clearScreen();

        // 计算当前等级属性
        BulletDamage = damageForm[weaponLevel-1];
        multiple = linesForm[weaponLevel-1];

        // 判断当前等级
        if (weaponLevel >= 1 && weaponLevel <= 3) {
            goldNeed = 30;
            weapon = "weapon1.txt";
        } else if (weaponLevel >= 4 && weaponLevel <= 6) {
            goldNeed = 40;
            weapon = "weapon2.txt";
        } else if (weaponLevel >= 7 && weaponLevel <= 9) {
            goldNeed = 50;
            weapon = "weapon3.txt";
        } else if (weaponLevel == 10) {
            // 最高等级显示
            vector<string> reachHighest = {
                "CONGRATULATIONS!",
                "You have reached the highest level!",
                "Current weapon level: " + to_string(weaponLevel),
                "Bullet Damage: " + to_string(BulletDamage),
                "Multiple: " + to_string(multiple),
                "Enter \"0\" to return to HOME"
            };
            displayMultiLineInBox("weapon3.txt", 32, 45, reachHighest);
            
            int choose;
            while (true) {
                cin >> choose;
                if (choose == 0) {
                    return;
                } else {
                    vector<string> invalidInput = {
                        "Invalid input!",
                        "Please enter 0 to return"
                    };
                    displayMultiLineInBox("weapon3.txt", 32, 55, invalidInput);
                }
            }
        }

        // 显示升级菜单
        vector<string> menu = {
            "Want to improve your combat power to better defeat your enemies?",
            "***It takes " + to_string(goldNeed) + "kg to upgrade to the next level***",
            "Enter \"1\" to advance to the next level",
            "Enter \"2\" to check current information",
            "Enter \"0\" to return to HOME"
        };
        displayMultiLineInBox(weapon, 32, 45, menu);

        int input;
        cin >> input;

        if (input == 1) {
            if (gold >= goldNeed) {
                gold -= goldNeed;
                weaponLevel++;
                BulletDamage = damageForm[weaponLevel-1];
                multiple = linesForm[weaponLevel-1];
                
                // 升级成功显示页面
                vector<string> upgradeSuccess = {
                    "CONGRATULATIONS!",
                    "Current weapon level: " + to_string(weaponLevel),
                    "Bullet Damage: " + to_string(BulletDamage),
                    "Multiple: " + to_string(multiple),
                    "Enter \"0\" to return"
                };
                displayMultiLineInBox(weapon, 32, 55, upgradeSuccess);
                
                int choose;
                while (true) {
                    cin >> choose;
                    if (choose == 0) break;
                    vector<string> invalidInput = {
                        "Invalid input!",
                        "Please enter 0 to return"
                    };
                    displayMultiLineInBox(weapon, 32, 55, invalidInput);
                }
            } else {
                // 金币不足提示
                vector<string> notEnoughGold = {
                    "Not enough gold to upgrade!",
                    "Enter \"0\" to return to HOME"
                };
                displayMultiLineInBox(weapon, 32, 55, notEnoughGold);
                
                int choose;
                while (true) {
                    cin >> choose;
                    if (choose == 0) break;
                    vector<string> invalidInput = {
                        "Invalid input!",
                        "Please enter 0 to return"
                    };
                    displayMultiLineInBox(weapon, 32, 55, invalidInput);
                }
            }
        } else if (input == 2) {
            vector<string> status = {
                "Current gold: " + to_string(gold),
                "Current weapon level: " + to_string(weaponLevel),
                "Bullet Damage: " + to_string(BulletDamage),
                "Multiple: " + to_string(multiple),
                "Enter \"0\" to return"
            };
            displayMultiLineInBox(weapon, 32, 55, status);
            
            int choose;
            while (true) {
                cin >> choose;
                if (choose == 0) break;
                vector<string> invalidInput = {
                    "Invalid input!",
                    "Please enter 0 to return"
                };
                displayMultiLineInBox(weapon, 32, 55, invalidInput);
            }
        } else if (input == 0) {
            return;
        } else {
            vector<string> invalidInput = {
                "Invalid input!",
                "Please enter 1, 2 or 0",
                "Enter \"0\" to return"
            };
            displayMultiLineInBox(weapon, 32, 55, invalidInput);
            
            int choose;
            while (true) {
                cin >> choose;
                if (choose == 0) break;
                vector<string> invalidInput = {
                    "Invalid input!",
                    "Please enter 0 to return"
                };
                displayMultiLineInBox(weapon, 32, 55, invalidInput);
            }
        }
    }
}
