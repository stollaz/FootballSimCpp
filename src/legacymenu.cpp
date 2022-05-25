#include <iostream>
#include <string>
#include <stdlib.h>
#include "fmt/core.h"
#include "fmt/format.h"

using namespace std;
#define VERSION "a.3.2022.05.25.0"

int StartMenu(){
    system("cls");
    int option = 1;

    fmt::print("\nWelcome to Bad Football Simulator Version {}!\n",VERSION);
    cout << "Options:" << endl;
    cout << "0. Simulate Random Season" << endl;
    cout << "1. Generate Random Teams" << endl;
    cout << "2. Load Teams" << endl;
    cout << "3. Simulate League Season (Skip to End)" << endl;
    cout << "4. Simulate League Season (Step by Step)" << endl;
    cout << "5. Simulate Single League Game" << endl;
    cout << "6. Debug (Testing Combinations)" << endl;
    cout << "9. Quit:" << endl;
    cout << "---\n> ";
    cin >> option;

    return option;
}

int main(){
    fmt::print("FOOTBALL SIMULATOR VERSION {}\n\n",VERSION);

    // Player p = Player("Test Goalkeeper", 1, Position::GK, 0,20,40,60,80,99);
    // p.DisplayStats();
    // cout << "\n";
    // Player q = Player("Striker Test", 9, Position::ST, 70,90,40,60,60,10);
    // q.DisplayStats();

    // PlayerInGame p_ = PlayerInGame(p);
    // p_.GiveCard();
    // p_.GiveCard();

    // Team t = Team("TestTeam");
    // t.AddPlayer(p);
    // // t.SaveTeam();

    // Goal g = Goal("Ronaldo", "Manchester United", 7);
    // g.GoalInfo();

    /*
    TODO:
    - More header files for each type of function, e.g.:
    - "generatestats.h" for GenerateFinishing() etc.
    - "utils.h" for GenerateNormal(), Sigmoid(),
    - "generateteam.h", "generateplayer.h"
    - "gamesimulation.h" for AttemptShot() etc.
    - "seasonsimulation.h" for SimulateSeason() etc.
    */

    system("cls"); // Clear console
    while(true){
        int option = StartMenu();
        switch(option){
            case 0:
                fmt::print("Simulate Random Season not yet coded.\n"); 
                system("pause");
                break;
            case 1:
                fmt::print("Set up teams not yet coded.\n"); 
                system("pause");
                break;
            case 2:
                fmt::print("Load teams not yet coded.\n"); 
                system("pause");
                break;
            case 3:
                fmt::print("Simulate League Season not yet coded.\n"); 
                system("pause");
                break;
            case 4:
                fmt::print("Simulate League Season not yet coded.\n"); 
                system("pause");
                break;
            case 5:
                fmt::print("Simulate League Game not yet coded.\n"); 
                system("pause");
                break;
            case 6:
                fmt::print("Debug not yet coded.\n"); 
                system("pause");
                break;
            case 9:
                fmt::print("Quitting...\n");
                exit(0);
            default:
                fmt::print("Unknown option.\n"); 
                system("pause");
                break;
        }
    }
    
}