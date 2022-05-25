#include <iostream>
#include <string>
#include <stdlib.h>

#include "player.cpp"
#include "playeringame.cpp"
#include "team.cpp"
#include "rotatableteam.cpp"
#include "goal.cpp"
#include "teamseasonstats.cpp"
#include "teamgamestats.cpp"
#include "logitem.cpp"
#include "game.cpp"
#include "gameweek.cpp"
#include "playerseasonstats.cpp"
#include "data.h"

using namespace std;
#define VERSION "a.3.2022.05.25.1"

void Debug(){
    Player p = Player("Test Goalkeeper", 1, Position::GK, 0,20,40,60,80,99);
    p.DisplayStats();
    cout << "\n";
    Player q = Player("Striker Test", 9, Position::ST, 70,90,40,60,60,10);
    q.DisplayStats();

    PlayerInGame p_ = PlayerInGame(p);
    p_.GiveCard();
    p_.GiveCard();

    Team t = Team("TestTeam");
    t.AddPlayer(p);
    // t.SaveTeam();

    Goal g = Goal("Ronaldo", "Manchester United", 7);
    g.GoalInfo();

    /*
    TODO:
    - More header files for each type of function, e.g.:
    - "generatestats.h" for GenerateFinishing() etc.
    - "utils.h" for GenerateNormal(), Sigmoid(),
    - "generateteam.h", "generateplayer.h"
    - "gamesimulation.h" for AttemptShot() etc.
    - "seasonsimulation.h" for SimulateSeason() etc.
    */
}

// This should go in utils
// Function to check if a string contains only digits (e.g. it is an int)
bool isStringInt(string s) {
    for (int i = 0; i < s.length(); i++){
        if (!isdigit(s[i])) return false; // Currently this only checks if every char is a digit, could extend to include '.' and '-'
    }
    return true;
}

// Print simulate one-off game menu choices and get input from user
int SimOneMenu(){
    system("cls");
    int option = -1;
    string input;

    fmt::print("\nBad Football Simulator Version {}\n",VERSION);
    fmt::print("\nSimulate One-Off Match mode selected.\n");
    cout << "Options:" << endl;
    cout << "1. Generate Random Teams" << endl;
    cout << "2. Choose Real Teams" << endl;
    cout << "3. Load Custom Teams" << endl;
    cout << "0. Back:" << endl;
    cout << "---\n> ";
    cin >> input;

    if (!isStringInt(input)) option = -1; // If input is not a number set to default
    else if (stoi(input) > 9 || stoi(input) < 0) option = -1; // If input is number but out of range set to default
    else option = stoi(input); // Otherwise convert to integer

    return option;
}

// Print simulate season menu choices and get input from user
int SimSeasonMenu(){
    system("cls");
    int option = -1;
    string input;

    fmt::print("\nBad Football Simulator Version {}\n",VERSION);
    fmt::print("\nSimulate Full Season mode selected.\n");
    cout << "Options:" << endl;
    cout << "1. Simulate with Random Teams" << endl;
    cout << "2. Simulate with Real League" << endl;
    cout << "0. Back:" << endl;
    cout << "---\n> ";
    cin >> input;

    if (!isStringInt(input)) option = -1; // If input is not a number set to default
    else if (stoi(input) > 9 || stoi(input) < 0) option = -1; // If input is number but out of range set to default
    else option = stoi(input); // Otherwise convert to integer

    return option;
}

// Begin simulating one-off game setup
void SimulateOne(){
    system("cls"); // Clear console
    bool valid = false;
    while(!valid){
        int option = SimOneMenu();
        switch(option){
            case 1:
                fmt::print("Generate Random Teams not yet coded.\n");
                valid = true;
                system("pause");
                break;
            case 2:
                fmt::print("Choose Real Teams not yet coded.\n");
                valid = true;
                system("pause");
                break;
            case 3:
                fmt::print("Load Custom Teams not yet coded.\n");
                valid = true;
                system("pause");
                break;
            case 0:
                valid = true;
                break;
            default:
                fmt::print("Unknown option.\n"); 
                system("pause");
                break;
        }
    }
}

// Begin simulating season setup
void SimulateSeason(){
    system("cls"); // Clear console
    bool valid = false;
    while(!valid){
        int option = SimSeasonMenu();
        switch(option){
            case 1:
                fmt::print("Simulate with Random Teams not yet coded.\n");
                valid = true;
                system("pause");
                break;
            case 2:
                fmt::print("Simulate Real League not yet coded.\n");
                valid = true;
                system("pause");
                break;
            case 0:
                valid = true;
                break;
            default:
                fmt::print("Unknown option.\n"); 
                system("pause");
                break;
        }
    }
}

// Print start menu choices and get input from user
int StartMenu(){
    system("cls");
    int option = -1;
    string input;

    fmt::print("\nWelcome to Bad Football Simulator Version {}!\n\n\n",VERSION);
    cout << "Options:" << endl;
    cout << "1. Simulate One-Off Match" << endl;
    cout << "2. Simulate Full Season" << endl;
    cout << "9. Debug (Testing Combinations)" << endl;
    cout << "0. Quit:" << endl;
    cout << "---\n> ";
    cin >> input;

    if (!isStringInt(input)) option = -1; // If input is not a number set to default
    else if (stoi(input) > 9 || stoi(input) < 0) option = -1; // If input is number but out of range set to default
    else option = stoi(input); // Otherwise convert to integer

    return option;
}

// Main Start Point
int main(){
    fmt::print("FOOTBALL SIMULATOR VERSION {}\n\n",VERSION);

    system("cls"); // Clear console
    while(true){
        int option = StartMenu();
        switch(option){
            case 1:
                fmt::print("Simulate One-Off Match not yet coded.\n"); 
                SimulateOne();
                // system("pause");
                break;
            case 2:
                fmt::print("Simulate Full Season not yet coded.\n");
                SimulateSeason();
                // system("pause");
                break;
            case 9:
                fmt::print("Debug Menu.\n");
                Debug();
                system("pause");
                break;
            case 0:
                fmt::print("Quitting...\n");
                exit(0);
            default:
                fmt::print("Unknown option.\n"); 
                system("pause");
                break;
        }
    }
    
}