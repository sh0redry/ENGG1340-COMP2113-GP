#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void explore(int number_of_people, int &gold, int &crop, int &people){
  srand(time(NULL));
  int randomValue=rand()%100;
  if (randomValue<30){
    int addGold=rand()%20 + 20*number_of_people;
    cout << "Congrats! You get " << addGold << " GOLD!" << endl;
    gold += addGold;
  }
  else if (randomValue>30 && randomValue<60){
    int price=100+100*people;
    int addCrop=rand()%50+3*price*number_of_people;
    cout << "Congrats! You get " << addCrop << " CROP!" << endl;
    crop += addCrop;
  }
  else if (randomValue>59 && randomValue<90){
    int addPeople=rand()%3+2;
    cout << "Congrats! You get " << addPeople << " PEOPLE!" << endl;
    people += addPeople;
  }
  else if (randomValue>89){
    cout << "AHHHHH! They die!" << endl;
    people -= number_of_people;
  }
}


