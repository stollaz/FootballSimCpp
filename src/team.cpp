#include "team.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

Team::Team(){}

// Constructor to create empty team with given name
Team::Team(std::string _name){
    name = _name;
    InitialiseTeam();
}

Team::Team(const Team& _team){
    name = _team.name;
    for (int i = 0; i < 11; i++){
        players[i] = _team.players[i];
    }
    rating = _team.rating;
}

// Add a specified player to the team, with the index of their number-1
void Team::AddPlayer(Player p){
    const int index = p.number -1;
    if (!players[index].Equals(null_player)) fmt::print("Player with this number already exists. Try again.\n");
    else players[index] = p;
}

// Initialise the team with null players
void Team::InitialiseTeam(){
    for (int i = 0; i < 11; i++) players[i] = null_player;
}

// Calculate the average rating of the team
void Team::CalculateRating(){
    int ovr = 0;
    for (Player p : players) ovr += p.overall;
    ovr /= 11;
    rating = ovr;
}

// Save the team to a file
void Team::SaveTeam(){
    int count = 1;
    const std::string fullPath = "assets/teams/" + name + ".txt";

    const std::string fileNameOnly = name;
    const std::string extension = ".txt";
    const std::string path = "assets/teams/";
    
    std::string newFullPath = fullPath;

    while (std::ifstream(newFullPath)){
        std::string tempFileName = fmt::format("{}({})",fileNameOnly,count++);
        newFullPath = path + tempFileName + extension;
    }

    std::ofstream writer(newFullPath);
    for (Player p : players) writer << p.PrintingRepresentation() << std::endl;

    writer.close();
    fmt::print("Saved file with path {}\n",newFullPath);
}

constexpr Team &Team::operator=(Team &_team){
    return _team;
}