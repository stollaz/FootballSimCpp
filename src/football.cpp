#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <locale>
#include <time.h>
#include <locale.h>
#include <io.h>
#include <fcntl.h>

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
#include "simulate.h"

#include "fmt/xchar.h"

using namespace std;
#define VERSION "a.3.2022.06.23.2"

#include <filesystem>
namespace fs = filesystem;

// Function definitions
int GetValue(string s, vector<tuple<string,int>> stats);
Position ConvertToPosition(string s);

Player GenerateRealPlayer(fs::path p);
vector<RotatableTeam> GenerateLeague();

string GeneratePlayerName();

void PrintTeam(Player t[]);

void Debug();
int SimOneMenu();
int SimSeasonMenu();
void SimulateOne();
void SimulateSeason();
int StartMenu();

int main();

/*
TODO

Mupltiple ways to simulate games
- Either do the same method as a.2, with simulating some random action every X minutes (fixed step)
- Do the above but with variable size timesteps, and not alternating teams, to emulate control / posession / randomness
- Or do a full simulation of the entire 90 minutes, in terms of ticks (new  simulation every X timesteps e.g. 1 second)
    - This would be very intensive but would allow for more in depth simulation to experiment with
    - e.g. actual physics, player movement, etc.
    - Would be very hard to get right, and would need to explore how to actually print events out here

Before simulation functionality, implement csv reading and team creation from file, as well as random team generation
*/

// Get value of a given stat from the given stats vector
int GetValue(string s, vector<tuple<string,int>> stats){
    for (tuple<string,int> stat : stats){
        if (get<0>(stat) == s) return get<1>(stat);
    }
    return -1;
}

// Convert a string position to a Position enum
Position ConvertToPosition(string s){
    if (s == "GK") return Position::GK;
    else if (s == "LB") return Position::LB;
    else if (s == "RB") return Position::RB;
    else if (s == "CB") return Position::CB;
    else if (s == "LCB") return Position::CB;
    else if (s == "RCB") return Position::CB;
    else if (s == "LM") return Position::LM;
    else if (s == "RM") return Position::RM;
    else if (s == "CM") return Position::CM;
    else if (s == "AM") return Position::AM;
    else if (s == "DM") return Position::DM;
    else if (s == "LCM") return Position::CM;
    else if (s == "RCM") return Position::CM;
    else if (s == "LDM") return Position::DM;
    else if (s == "RDM") return Position::DM;
    else if (s == "LAM") return Position::AM;
    else if (s == "RAM") return Position::AM;
    else if (s == "LW") return Position::LW;
    else if (s == "RW") return Position::RW;
    else if (s == "ST") return Position::ST;
    else if (s == "CF") return Position::ST;
    else if (s == "DF") return Position::DF;
    else if (s == "MF") return Position::MF;
    else if (s == "FW") return Position::FW;
    else if (s == "WM") return Position::LM;
    else if (s == "WB") return Position::LB;
    else if (s == "WF") return Position::LW;
    else return Position::MF;
}

// Function to generate stats for real player based on csv and return said player
Player GenerateRealPlayer(fs::path p){
    vector<string> lines = ReadAllLines(p);  // Get all lines of file as vector of strings

    // for (string line : lines) cout << line << endl;

    vector<string> info = Split(lines[0],','); // Basic info

    // Create name
    string forename = info[1];
    string surname = info[0];
    string fullname = "";
    if (forename == "") fullname = surname;
    else fullname = forename + " " + surname;

    // TODO Convert fullname to UTF-8 / normalised representation

    // Create shirt name
    string shirtname = "";
    if (forename == "") shirtname = surname;
    else shirtname = forename[0] + ". " + surname;

    string team = info[2]; // Load team

    // Load positions
    vector<string> positions = Split(lines[1],',');
    // for (string pos : positions) cout << pos << endl;
    string pos1 = positions[0];
    string pos2 = positions[1];
    //string pos3 = positions[2];

    // Initialise stats
    // int FW_TOT = 0;
    // int FW_AVG = 0;
    // int MF_TOT = 0;
    // int MF_AVG = 0;
    // int DF_TOT = 0;
    // int DF_AVG = 0;
    // int GK_TOT = 0;
    // int GK_AVG = 0;

    int DRIBBLING = 0;
    int PASSING = 0;
    int ASSISTING = 0;
    int FINISHING = 0;
    int TACKLING = 0;
    int GOAL_PREVENTION = 0;

    vector<tuple<string,int>> stats; // Vector of string-int stat tuples

    size_t i = 3;
    while (i < lines.size()){
        vector<string> line = Split(lines[i],','); // Split stat line

        // Find which positional type this stat falls under and increment relevant counter
        // The `find` function checks if a value is in a vector - maybe make this its own utility too
        // if (find(FW_DATA.begin(), FW_DATA.end(), line[0]) != FW_DATA.end()) FW_TOT += stoi(line[2]);
        // else if (find(MF_DATA.begin(), MF_DATA.end(), line[0]) != MF_DATA.end()) MF_TOT += stoi(line[2]);
        // else if (find(DF_DATA.begin(), DF_DATA.end(), line[0]) != DF_DATA.end()) DF_TOT += stoi(line[2]);
        // else if (find(GK_DATA.begin(), GK_DATA.end(), line[0]) != GK_DATA.end()) GK_TOT += stoi(line[2]);

        tuple<string,int> t = make_tuple(line[0],stoi(line[2]));
        if (find(DATA.begin(), DATA.end(), line[0]) != DATA.end()) stats.push_back(t);

        i++;
    }

    // TODO: Actually parse stats and save into Player
    // This requires rounding, which is difficult in C++, so research needs to be done
    if (pos1 == "GK"){
        // GK_AVG = GK_TOT/13;

        DRIBBLING = (0.5 * GetValue("DEFENSIVE_ACTIONS_OUTSIDE_AREA", stats));
        PASSING = (0.25 * GetValue("TOUCHES", stats) + 0.25 * GetValue("LAUNCH_PERCENTAGE", stats) + 0.25 * GetValue("GOAL_KICKS", stats) + 0.25 * GetValue("AVG_GOAL_KICK_DISTANCE", stats));
        ASSISTING = (0.5 * GetValue("GOAL_KICKS", stats) + 0.5 * GetValue("AVG_GOAL_KICK_DISTANCE", stats));
        FINISHING = (0.5 * GetValue("AVG_GOAL_KICK_DISTANCE", stats));
        TACKLING = (0.4 * GetValue("CROSSES_STOPPED_PERCENTAGE", stats) + 0.4 * GetValue("DEFENSIVE_ACTIONS_OUTSIDE_AREA", stats) + 0.2 * GetValue("AVG_DISTANCE_OF_DEFENSIVE_ACTIONS", stats));
        GOAL_PREVENTION = (0.15 * GetValue("POST_SHOT_XG-CONCEDED", stats) + 0.15 * GetValue("GOALS_CONCEDED", stats) + 0.25 * GetValue("SAVE_PERCENTAGE", stats) + 0.15 * GetValue("POST_SHOT_XG_PER_SHOT_ON_TARGET", stats) + 0.1 * GetValue("PENALTY_SAVE_PERCENTAGE", stats) + 0.2 * GetValue("CLEAN_SHEET_PERCENTAGE", stats));
    }
    else{
        // FW_AVG = FW_TOT / 7;
        // MF_AVG = MF_TOT / 7;
        // DF_AVG = DF_TOT / 6;
        
        DRIBBLING = (0.4 * GetValue("PROGRESSIVE CARRIES", stats) + 0.4 * GetValue("DRIBBLES_COMPLETED", stats) + 0.1 * GetValue("TOUCHES_IN_AREA", stats) + 0.1 * GetValue("PROGRESSIVE_PASSES_RECIEVED", stats));
        PASSING = (0.3 * GetValue("PASSES_ATTEMPTED", stats) + 0.4 * GetValue("PASS_COMPLETION_PERCENTAGE", stats) + 0.3 * GetValue("PROGRESSIVE_PASSES", stats));
        ASSISTING = (0.2 * GetValue("ASSISTS", stats) + 0.3 * GetValue("XA", stats) + 0.1 * GetValue("PROGRESSIVE_PASSES", stats) + 0.3 * GetValue("SHOT_CREATIONS", stats) + 0.1 * GetValue("NON_PEN_XG+XA", stats));
        FINISHING = (0.4 * GetValue("NON_PEN_GOALS", stats) + 0.3 * GetValue("NON_PEN_XG", stats) + 0.2 * GetValue("TOTAL_SHOTS", stats) + 0.1 * GetValue("NON_PEN_XG+XA", stats));
        TACKLING = (0.15 * GetValue("PRESSURES", stats) + 0.35 * GetValue("INTERCEPTIONS", stats) + 0.35 * GetValue("TACKLES", stats) + 0.15 * GetValue("AERIAL_DUELS_WON", stats));
        GOAL_PREVENTION = (0.5 * GetValue("BLOCKS", stats) + 0.5 * GetValue("CLEARANCES", stats));
    }

    // TODO: Implement ConvertToPosition to convert string (e.g. "GK") to position (Position::GK)
    Player player = Player(fullname,0,ConvertToPosition(pos1), DRIBBLING, FINISHING, TACKLING, PASSING, ASSISTING, GOAL_PREVENTION);

    return player;
}

// Function to generate all players and teams in a league and return a vector of those teams
vector<RotatableTeam> GenerateLeague(){
    vector<RotatableTeam> ret; // Dummy return
    string regions[5] = {"ENG","ESP","FRA","ITA","GER"}; // List of regions
    // Print and ask for input
    // Validate and default if needed
    string r = "ENG"; // Default

    string root = "assets/csvs/" + r + "1"; // Setup root directory for that region
    vector<fs::path> paths; // Create vector of paths for each team in that league
    vector<RotatableTeam> TEAMS;
    // Loop through each file in this directory, adding that path to the vector
    for (const auto & entry : fs::directory_iterator(root)) {
        paths.push_back(entry.path()); 
        // cout << entry.path() << endl;
    }

    for (auto team : paths){ // For each path in the vector
        string teamname = team.string().substr(team.string().find_last_of("/\\") + 1); // Create team name from last part of path
        // cout << teamname << endl;
        RotatableTeam t = RotatableTeam(teamname); // Initialise new team
        // cout << "Initialised team" << endl;
        for (const auto & entry : fs::directory_iterator(team)){ // For each player in this team (file in directory)
            // cout << entry.path() << endl;
            Player a = GenerateRealPlayer(entry); // Generate real player from that file
            t.AddPlayer(a); // Add player to team
        }
        t.GenerateBestXI();
        TEAMS.push_back(t);
    }

    return TEAMS;
}

// Method to print out a team in both positional representation and as a list of names
void PrintTeam(Player t[]){
    fmt::print("-------");
    if (t[9-1].number < 10) fmt::print("-[{}]",t[9-1].number); else fmt::print("[{}]",t[9-1].number) ; // ST
    fmt::print("-------");
    fmt::print("\n------------------\n");
    if (t[7-1].number < 10) fmt::print("[{}]-",t[7-1].number); else fmt::print("[{}]",t[7-1].number) ; // LM
    fmt::print("---");
    if (t[10-1].number < 10) fmt::print("-[{}]",t[10-1].number); else fmt::print("[{}]",t[10-1].number) ; // AM
    fmt::print("---");
    if (t[11-1].number < 10) fmt::print("-[{}]",t[11-1].number); else fmt::print("[{}]",t[11-1].number) ; // RM
    fmt::print("\n------------------\n");
    fmt::print("---");
    if (t[6-1].number < 10) fmt::print("-[{}]",t[6-1].number); else fmt::print("[{}]",t[6-1].number) ; // DM
    fmt::print("----");
    if (t[8-1].number < 10) fmt::print("-[{}]",t[8-1].number); else fmt::print("[{}]",t[8-1].number) ; // DM
    fmt::print("---");
    fmt::print("\n------------------\n");
    if (t[2-1].number < 10) fmt::print("[{}]-",t[2-1].number); else fmt::print("[{}]",t[2-1].number) ; // LB
    fmt::print("-");
    if (t[4-1].number < 10) fmt::print("-[{}]",t[4-1].number); else fmt::print("[{}]",t[4-1].number) ; // CB
    if (t[5-1].number < 10) fmt::print("-[{}]",t[5-1].number); else fmt::print("[{}]",t[5-1].number) ; // CB
    fmt::print("-");
    if (t[3-1].number < 10) fmt::print("-[{}]",t[3-1].number); else fmt::print("[{}]",t[3-1].number) ; // RB
    fmt::print("\n-------");
    if (t[1-1].number < 10) fmt::print("-[{}]",t[1-1].number); else fmt::print("[{}]",t[1-1].number) ; // GK
    fmt::print("-------\n");
}

void PrintPitch(bool bg_color = false){
    fmt::color red = fmt::color::red;
    fmt::color blue = fmt::color::blue;
    fmt::color green = fmt::color::dark_green;

    // Pitch shape - one charatcer (y) = 5 metres, (x) = 2.5m
    // 105x65 -> 43x13
    vector<wstring> PITCH;
    PITCH.push_back(L"┌─────────────────────┬─────────────────────┐");
    PITCH.push_back(L"│                     │                     │");
    PITCH.push_back(L"│                     │                     │");
    PITCH.push_back(L"├─────┐               │               ┌─────┤");
    PITCH.push_back(L"│     │               │               │     │");
    PITCH.push_back(L"│     │               │               │     │");
    PITCH.push_back(L"│     ├╮            ╭─┼─╮            ╭┤     │");
    PITCH.push_back(L"│   ● ││            │ │ │            ││ ●   │");
    PITCH.push_back(L"│     ├╯            ╰─┼─╯            ╰┤     │");
    PITCH.push_back(L"│     │               │               │     │");
    PITCH.push_back(L"│     │               │               │     │");
    PITCH.push_back(L"├─────┘               │               └─────┤");
    PITCH.push_back(L"│                     │                     │");
    PITCH.push_back(L"│                     │                     │");
    PITCH.push_back(L"└─────────────────────┴─────────────────────┘");

    int players[13][43];
    for (int i = 0; i < 13; i++){
        for (int j = 0; j < 43; j++){
            players[i][j] = 0;
        }
    }

    players[1][4] = 1;
    players[1][42-4] = 2;

    players[1][17] = 1;
    players[1][42-17] = 2;

    players[6][18] = 1;
    players[6][42-18] = 2;

    players[6][11] = 1;
    players[6][42-11] = 2;

    players[4][13] = 1;
    players[4][42-13] = 2;

    players[8][13] = 1;
    players[8][42-13] = 2;

    players[4][4] = 1;
    players[4][42-4] = 2;

    players[8][4] = 1;
    players[8][42-4] = 2;

    players[11][4] = 1;
    players[11][42-4] = 2;

    players[11][17] = 1;
    players[11][42-17] = 2;

    players[6][0] = 1;
    players[6][42-0] = 2;

    
    _setmode(_fileno(stdout), 0x00020000);
    if (bg_color){
        fmt::print(bg(green),L"┌─────────────────────┬─────────────────────┐\n");
        for (int i = 0; i < 13; i++){
            if (i == 2 || i == 10) fmt::print(bg(green),L"├");
            else fmt::print(bg(green),L"│");
            for (int j = 0; j < 43; j++){
                // _setmode(_fileno(stdout), 0x00020000);
                if (players[i][j] == 0) fmt::print(bg(green),L"{}",PITCH[i+1][j+1]);
                if (players[i][j] == 1) fmt::print(bg(green) | fg(red),L"■");
                if (players[i][j] == 2) fmt::print(bg(green) | fg(blue),L"■");
                // _setmode(_fileno(stdout), 0x4000);
            }
            if (i == 2 || i == 10) fmt::print(bg(green),L"┤");
            else fmt::print(bg(green),L"│");
            fmt::print(L"\n");
        }
        fmt::print(bg(green),L"└─────────────────────┴─────────────────────┘");
        fmt::print(L"\n");
    }
    else{
        fmt::print(L"┌─────────────────────┬─────────────────────┐\n");
        for (int i = 0; i < 13; i++){
            if (i == 2 || i == 10) fmt::print(L"├");
            else fmt::print(L"│");
            for (int j = 0; j < 43; j++){
                // _setmode(_fileno(stdout), 0x00020000);
                if (players[i][j] == 0) fmt::print(L"{}",PITCH[i+1][j+1]);
                if (players[i][j] == 1) fmt::print(fg(red),L"■");
                if (players[i][j] == 2) fmt::print(fg(blue),L"■");
                // _setmode(_fileno(stdout), 0x4000);
            }
            if (i == 2 || i == 10) fmt::print(L"┤");
            else fmt::print(L"│");
            fmt::print(L"\n");
        }
        fmt::print(L"└─────────────────────┴─────────────────────┘");
        fmt::print(L"\n");
    }
    _setmode(_fileno(stdout), 0x4000);
}

void TestMatch(){
    Team team1 = Team(GenerateTeamName());

    team1.AddPlayer(GeneratePlayer(Position::GK,1));
    team1.AddPlayer(GeneratePlayer(Position::LB,2));
    team1.AddPlayer(GeneratePlayer(Position::RB,3));
    team1.AddPlayer(GeneratePlayer(Position::CB,4));
    team1.AddPlayer(GeneratePlayer(Position::CB,5));
    team1.AddPlayer(GeneratePlayer(Position::DM,6));
    team1.AddPlayer(GeneratePlayer(Position::LW,7));
    team1.AddPlayer(GeneratePlayer(Position::CM,8));
    team1.AddPlayer(GeneratePlayer(Position::ST,9));
    team1.AddPlayer(GeneratePlayer(Position::CM,10));
    team1.AddPlayer(GeneratePlayer(Position::RW,11));

    Team team2 = Team(GenerateTeamName());

    team2.AddPlayer(GeneratePlayer(Position::GK,1));
    team2.AddPlayer(GeneratePlayer(Position::LB,2));
    team2.AddPlayer(GeneratePlayer(Position::RB,3));
    team2.AddPlayer(GeneratePlayer(Position::CB,4));
    team2.AddPlayer(GeneratePlayer(Position::CB,5));
    team2.AddPlayer(GeneratePlayer(Position::DM,6));
    team2.AddPlayer(GeneratePlayer(Position::LW,7));
    team2.AddPlayer(GeneratePlayer(Position::CM,8));
    team2.AddPlayer(GeneratePlayer(Position::ST,9));
    team2.AddPlayer(GeneratePlayer(Position::CM,10));
    team2.AddPlayer(GeneratePlayer(Position::RW,11));

    TeamGameStats t1 = TeamGameStats(team1);
    TeamGameStats t2 = TeamGameStats(team2);

    vector<PlayerInGame> p1s;
    for (Player p : t1.team.players) p1s.push_back(PlayerInGame(p));
    t1.players = p1s;
    vector<PlayerInGame> p2s;
    for (Player p : t2.team.players) p2s.push_back(PlayerInGame(p));
    t2.players = p2s;

    Game test = Game("0",team1, team2, t1, t2);

    SimulateGame(team1, team2, true);

    // fmt::print("Team 1: {}\n",t1.team.name);
    // for (Player p : t1.team.players) fmt::print("{}\n",p.ToString());

    // fmt::print("\nTeam 2: {}\n",t2.team.name);
    // for (Player p : t2.team.players) fmt::print("{}\n",p.ToString());

    // fmt::print("\n\n");

    // AttemptPass(test, t1.players[6],t1.players[10],t1.players[5],true);
    // test.AddBlankLine();
    // AttemptShot(test, t1, t2, t1.players[8],true);
    // test.AddBlankLine();
    // AttemptSetPiece(test, t1, t2, true, true);
    // test.AddBlankLine();
    // GiveCard(test, t1.players[10]);
    // test.AddBlankLine();
    // GiveCard(test, t1.players[10]);
    // test.AddBlankLine();
    // test.AddRead();

    // test.PrintLog();

    // for (PlayerInGame p : t1.players) PrintRatingWithColour(p);
    // fmt::print("\n");
    // for (PlayerInGame p : t2.players) PrintRatingWithColour(p);
}

int DebugMenu(){
    system("cls");
    int option = -1;
    string input;

    fmt::print("\nBad Football Simulator Version {}\n",VERSION);
    fmt::print("\nDebug Mode.\n");
    cout << "Options:" << endl;
    cout << "1. Simulate Test Match" << endl;
    cout << "2. Display Pitch (no grass)" << endl;
    cout << "3. Display Pitch (with grass)" << endl;
    cout << "4. Test" << endl;
    cout << "0. Back:" << endl;
    cout << "---\n> ";
    cin >> input;

    if (!isStringInt(input)) option = -1; // If input is not a number set to default
    else if (stoi(input) > 9 || stoi(input) < 0) option = -1; // If input is number but out of range set to default
    else option = stoi(input); // Otherwise convert to integer

    return option;
}

void Test(){
    int x = 5;
    fmt::print("Init: x = {}\n", x);
    int *y;
    y = &x;
    fmt::print("Pointer: y = {}\n",fmt::ptr(y));
    (*y)++;
    fmt::print("Incremented: y = {}, x = {}\n", fmt::ptr(y), x);
}

void Debug(){
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

    // RotatableTeam t = RotatableTeam("test");

    // vector<RotatableTeam> teams = GenerateLeague();

    // for (RotatableTeam team : teams) {
    //     // fmt::print("Best XI for {}:\n",team.name);
    //     fmt::print("Players in {}: {}\n",team.name, team.players.size());
    //     // for (Player p : team.players) cout << p.ToString() << endl;
    //     PrintTeam(team.bestXI);
    //     for (Player p : team.bestXI) cout << p.ToString() << endl;
    //     fmt::print("\n\n");
    // }

    // fmt::print("Random Name: {}\n",GeneratePlayerName());
    // fmt::print("Random Team: {}\n",GenerateTeamName());

    // for (double r = 4; r <= 10.0; r+=0.5) PrintRatingWithColour(r,true);

    system("cls"); // Clear console
    bool valid = false;
    while(!valid){
        int option = DebugMenu();
        switch(option){
            case 1:
                TestMatch();
                valid = true;
                system("pause");
                break;
            case 2:
                PrintPitch(false);
                valid = true;
                system("pause");
                break;
            case 3:
                PrintPitch(true);
                valid = true;
                system("pause");
                break;
            case 4:
                Test();
                valid = true;
                system("pause");
            case 0:
                valid = true;
                break;
            default:
                fmt::print("Unknown option.\n"); 
                system("pause");
                break;
        }
    }

    // default_random_engine gen = CreateGenerator();
    // normal_distribution<double> dist = CreateNormalGenerator(50,20);

    // for (int i = 0; i < 10; i++) {
    //     fmt::print("Random normal: {}\n",(int)dist(gen));
    // }

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
    cout << "9. Debug" << endl;
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
    srand(time(NULL)); // Initialise randomiser with seed

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
                // system("pause");
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