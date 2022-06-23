#include "rotatableteam.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>

RotatableTeam::RotatableTeam(){}

// Constructor to create empty team with given name
RotatableTeam::RotatableTeam(std::string _name){
    RotatableTeam::name = _name;
    RotatableTeam::InitialiseTeam();
}

// Add a specified player to the team, with the index of their number-1
void RotatableTeam::AddPlayer(Player p){
    players.push_back(p);
}

// Initialise the team with null players
void RotatableTeam::InitialiseTeam(){
    for (int i = 0; i < 11; i++) bestXI[i] = null_player;
}

// Calculate the average rating of the team
void RotatableTeam::CalculateRating(){
    int ovr = 0;
    for (Player p : players) ovr += p.overall;
    ovr /= players.size();
    rating = ovr;
}

void DeleteFromVector(std::vector<Player>& list, Player& target) 
{
    for (size_t i = 0; i < list.size(); i++){
        if (list[i].name == target.name) list.erase(list.begin()+i);
    }
}

// Very crude way of generating a "Best XI", likely not featuring the actual best possible set of players for a team
//      but hopefully a good approximation
// This NEEDS testing as it was ported from C# without testing
// Specifically the removal from lists is untested, but per help from https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
void RotatableTeam::GenerateBestXI(){
    // fmt::print("Generating best XI for {}\n",name);
    // Create vectors of all players in a given position type
    std::vector<Player> _GKS;
    std::vector<Player> _DFS;
    std::vector<Player> _MFS;
    std::vector<Player> _FWS;

    // Populate vectors
    for (Player p : players){
        if (p.PositionType(p.position) == Position::GK) _GKS.push_back(p);
        else if (p.PositionType(p.position) == Position::DF) _DFS.push_back(p);
        else if (p.PositionType(p.position) == Position::MF) _MFS.push_back(p);
        else if (p.PositionType(p.position) == Position::FW) _FWS.push_back(p);
    }

    // fmt::print("GKs Length is {}\n",_GKS.size());
    // fmt::print("DFs Length is {}\n",_DFS.size());
    // fmt::print("MFs Length is {}\n",_MFS.size());
    // fmt::print("FWs Length is {}\n",_FWS.size());

    // If the midfielders vector is too small (the most common case) consider secondary positions too
    if (_MFS.size() < 3){
        // fmt::print("---\n");
        for (Player p : players){
            if (p.PositionType(p.position2) == Position::MF) _MFS.push_back(p);
        }
    }

    // Create sorted vectors of players, ordered by given conditions relevant to that position
    std::vector<Player> OrderedGKS = _GKS;
    std::sort(OrderedGKS.begin(), OrderedGKS.end(), [](Player a, Player b){
        if (a.positionalOverall != b.positionalOverall) return a.positionalOverall < b.positionalOverall;
        return a.goalPrevention < b.goalPrevention;
    });

    std::vector<Player> OrderedDFS = _DFS;
    std::sort(OrderedDFS.begin(), OrderedDFS.end(), [](Player a, Player b){
        if (a.positionalOverall != b.positionalOverall) return a.positionalOverall < b.positionalOverall;
        return a.goalPrevention < b.goalPrevention;
    });

    std::vector<Player> OrderedMFS = _MFS;
    std::sort(OrderedMFS.begin(), OrderedMFS.end(), [](Player a, Player b){
        if (a.positionalOverall != b.positionalOverall) return a.positionalOverall < b.positionalOverall;
        return a.assisting < b.assisting;
    });

    std::vector<Player> OrderedFWS = _FWS;
    std::sort(OrderedFWS.begin(), OrderedFWS.end(), [](Player a, Player b){
        if (a.positionalOverall != b.positionalOverall) return a.positionalOverall < b.positionalOverall;
        return a.finishing < b.finishing;
    });
    
    // fmt::print("Sorted.\n");

    // Get best GK is easy, just get the first from the list
    OrderedGKS[0].number = 1;
    bestXI[0] = OrderedGKS[0];

    // Get best DFs
    
    // fmt::print("Getting best player...\n");
    bestXI[1] = GetBestPlayer(Position::LB, OrderedDFS);
    // fmt::print("Got best player done.\n");
    bestXI[1].number = 2;
    // Remove that player to avoid duplicates
    // OrderedDFS.erase(std::remove(OrderedDFS.begin(), OrderedDFS.end(),bestXI[1]),OrderedDFS.end());
    // fmt::print("Deleting from vector 1...\n");
    DeleteFromVector(OrderedDFS, bestXI[1]);
    // fmt::print("Deleted from vector 1 done\n");

    bestXI[2] = GetBestPlayer(Position::RB, OrderedDFS);
    bestXI[2].number = 3;
    // OrderedDFS.erase(std::remove(OrderedDFS.begin(), OrderedDFS.end(),bestXI[2]),OrderedDFS.end());
    DeleteFromVector(OrderedDFS, bestXI[2]);

    bestXI[3] = GetBestPlayer(Position::CB, OrderedDFS);
    bestXI[3].number = 4;
    // OrderedDFS.erase(std::remove(OrderedDFS.begin(), OrderedDFS.end(),bestXI[3]),OrderedDFS.end());
    DeleteFromVector(OrderedDFS, bestXI[3]);

    bestXI[4] = GetBestPlayer(Position::CB, OrderedDFS);
    bestXI[4].number = 5;
    // OrderedDFS.erase(std::remove(OrderedDFS.begin(), OrderedDFS.end(),bestXI[4]),OrderedDFS.end());
    DeleteFromVector(OrderedDFS, bestXI[4]);

    //---

    Player p6 = GetBestPlayer(Position::CM, OrderedMFS); // CM
    if (p6.name == "") p6 = GetBestPlayer(Position::DM, OrderedMFS); // DM if no CMs
    if (p6.name == "") p6 = GetBestPlayer(Position::AM, OrderedMFS); // AM if no CMs
    bestXI[5] = p6;
    bestXI[5].number = 6;
    // OrderedDFS.erase(std::remove(OrderedMFS.begin(), OrderedMFS.end(),bestXI[5]),OrderedMFS.end());
    DeleteFromVector(OrderedMFS, bestXI[5]);

    Player p8 = GetBestPlayer(Position::CM, OrderedMFS);
    if (p8.name == "") p8 = GetBestPlayer(Position::DM, OrderedMFS);
    if (p8.name == "") p8 = GetBestPlayer(Position::AM, OrderedMFS);
    bestXI[7] = p8;
    bestXI[7].number = 8;
    // OrderedDFS.erase(std::remove(OrderedMFS.begin(), OrderedMFS.end(),bestXI[7]),OrderedMFS.end());
    DeleteFromVector(OrderedMFS, bestXI[7]);

    if (OrderedMFS.size() == 0){
        bestXI[9] = GetBestPlayer(Position::AM, OrderedFWS);
        bestXI[9].number = 10;
        DeleteFromVector(OrderedFWS, bestXI[9]);
    }
    else{
        bestXI[9] = GetBestPlayer(Position::AM, OrderedMFS);
        bestXI[9].number = 10;
        // OrderedDFS.erase(std::remove(OrderedMFS.begin(), OrderedMFS.end(),bestXI[9]),OrderedMFS.end());
        DeleteFromVector(OrderedMFS, bestXI[9]);
    }

    bestXI[8] = GetBestPlayer(Position::ST, OrderedFWS); // ST
    bestXI[8].number = 9;
    // OrderedDFS.erase(std::remove(OrderedFWS.begin(), OrderedFWS.end(),bestXI[8]),OrderedFWS.end());
    DeleteFromVector(OrderedFWS, bestXI[8]);

    if (OrderedFWS.size() < 2){  // If not enough forwards
        bestXI[6] = GetBestPlayer(Position::LM, OrderedMFS);
        bestXI[6].number = 7;   // LW
        // OrderedDFS.erase(std::remove(OrderedMFS.begin(), OrderedMFS.end(),bestXI[6]),OrderedMFS.end());
    DeleteFromVector(OrderedMFS, bestXI[6]);

        bestXI[10] = GetBestPlayer(Position::RM, OrderedMFS);
        bestXI[10].number = 11;  // RW
        // OrderedDFS.erase(std::remove(OrderedMFS.begin(), OrderedMFS.end(),bestXI[10]),OrderedMFS.end());
    DeleteFromVector(OrderedMFS, bestXI[10]);
    }
    else{
        bestXI[6] = GetBestPlayer(Position::LW, OrderedFWS);
        bestXI[6].number = 7;   // LW
        // OrderedDFS.erase(std::remove(OrderedFWS.begin(), OrderedFWS.end(),bestXI[6]),OrderedFWS.end());
    DeleteFromVector(OrderedFWS, bestXI[6]);

        bestXI[10] = GetBestPlayer(Position::RW, OrderedFWS);
        bestXI[10].number = 11;  // RW
        // OrderedDFS.erase(std::remove(OrderedFWS.begin(), OrderedFWS.end(),bestXI[10]),OrderedFWS.end());
    DeleteFromVector(OrderedFWS, bestXI[10]);
    }

    // for (Player pl : bestXI) fmt::print("{}: {}\n",name,pl.name);
}

// Get the best player in a given position
Player RotatableTeam::GetBestPlayer(Position Pos, std::vector<Player> OrderedList){
    // fmt::print("List size is {}\n",OrderedList.size());
    Player ret = Player("",0,Position::GK,0,0,0,0,0,0); // Create temporary empty player
    bool pFound = false;
    size_t i = 0;

    // fmt::print("Finding best player: Start it 1\n");

    // Search the list for the "best" player that plays in the desired position
    while (!pFound && i < OrderedList.size()){
        Player p = OrderedList[i]; // Assign current player to temporary player object
        if (p.position == Pos || p.position2 == Pos){ // Check if either primary or secondary position matches
            pFound = true; // Mark as found and mark to return
            ret = p;
        }
        else i++; // Otherwise check next player
    }

    // fmt::print("Finding best player: Start it 2\n");

    if (!pFound){ // if the desired position does not exist in the list, try again but with a general check for position type
        i = 0;
        while (!pFound && i < OrderedList.size()){
            Player p = OrderedList[i];
            // Do generic positional checks
            if (p.PositionType(Pos) == Position::DF && (p.position == Position::DF || p.position2 == Position::DF)){ // DF 
                pFound = true;
                ret = p;
            }
            else if (p.PositionType(Pos) == Position::MF && (p.position == Position::MF || p.position2 == Position::MF)){ // MF
                pFound = true;
                ret = p;
            }
            else if (p.PositionType(Pos) == Position::FW && (p.position == Position::FW || p.position2 == Position::FW)){ // FW
                pFound = true;
                ret = p;
            } 
            else i++;
        }
    }

    // fmt::print("Finding best player: Start it 3\n");

    // if there is still no best player found, pick the best player in the list regardless of exact position
    if (!pFound){
        Player p = OrderedList[0];
        pFound = true;
        ret = p;
    }

    // fmt::print("Returning best player: {}\n",ret.name);

    return ret;
}

// Unused
// Player RotatableTeam::_GetBestPlayer(Position Pos, std::vector<Player> OrderedList){}

// Save the team to a file
void RotatableTeam::SaveTeam(){
    int count = 1;
    std::string fullPath = "assets/teams/" + name + ".txt";

    std::string fileNameOnly = "assets/teams/" + name;
    std::string extension = ".txt";
    std::string path = "assets/teams/";
    
    std::string newFullPath = fullPath;

    while (std::ifstream(newFullPath)){
        std::string tempFileName = fmt::format("{}({})",fileNameOnly,count++);
        newFullPath = path + tempFileName + extension;
    }

    std::ofstream writer(newFullPath);
    for (Player p : players) writer << p.PrintingRepresentation() << std::endl;

    writer.close();
    fmt::print("Saved file with path {}",newFullPath);
}