void counterImproveLevel(int &gold, int &level) {

    int maxUp = gold/300
    cout << "***It takes 300 gold to move up a level***" << endl;
    cout << "Current gold: " << $gold << endl;
    cout << "Maximum number of levels available: " << maxUp << endl;
    bool a = false;
    while (!a){   //检查输入是否有效
        cout << "Please enter the number of levels (1-" << maxUp << ") you want to move up :" << endl;
        cout << "Or enter \"0\" to quit this counter" << endl;
        int levelIncrease;
        cin >> levelIncrease;    //用户需要升的级数
        if (levelIncrease >= 1){    
            int cost = levelIncrease * 300   // 需要的金币数
    
            if (gold >= cost) {
                gold -= cost;           // 扣除金币
                level += levelIncrease;     // 增加等级
                a = true;
            }
            else {
                cout << "Not enough gold to level up!" << endl;
            }
        }
        else if (levelIncrease == 0){
            a = true; 
            //退出
        }
        else{
            cout << " Please enter the number of valid upgrades " <<endl;
            cout << "1-" << maxUp << " and a positive integer)" <<endl;
        }
    }
}

