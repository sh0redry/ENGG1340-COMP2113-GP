#include <iostream>
using namespace std;

print_file()

int explore(int difficulty, int people_available, int number_of_people, int &gold, int &crop, int &people){
  srand(time(NULL));
  int randomValue=rand()%100;
  if (randomValue<30){
    int addGold=rand()%20 + 10*(4-difficulty)*number_of_people;
    gold += addGold;
  }
  else if (randomValue>30 && randomValue<60){
    int price=100+100*people;
    int addCrop=rand()%50+(4-difficulty)*price*number_of_people;
    crop += addCrop;
  }
  else if (randomValue>59 && randomValue<90){
    int addPeople=rand()%3+4-difficulty;
    people += addPeople;
  }
  else if (randomValue>89){
    people -= number_of_people;
  }
}

int main(){
  print_file(explore1.txt); // print out the first map and wait until Enter
  print_file(explore2.txt); // print out the second map and input the num of people
  if (input > people_available){
    cout << "invalid input" << endl;
  else{
  explore(difficulty, people_available, number_of_people, gold, crop, people);
  if (// the outcome is positive){
    print_file(explore3.txt);
  }
  else{
    print_file(explore4.txt);
  }
  }
  return 0;
}

  
