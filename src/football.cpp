#include <iostream>
#include <string>
#include "player.cpp"
#include "playeringame.cpp"

using namespace std;
#define VERSION "a.3.2022.05.24.1"

int main(){
    fmt::print("FOOTBALL SIMULATOR VERSION {}\n\n",VERSION);
    Player p = Player("Test Goalkeeper", 1, Position::GK, 0,20,40,60,80,99);
    p.DisplayStats();
    // cout << "\n";
    // Player q = Player("Striker Test", 9, Position::ST, 70,90,40,60,60,10);
    // q.DisplayStats();

    PlayerInGame p_ = PlayerInGame(p);
    p_.GiveCard();
    p_.GiveCard();
}