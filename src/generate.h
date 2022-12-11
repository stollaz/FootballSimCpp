#include <string>
#include <random>
#include <vector>
#include "fmt/core.h"
#include "fmt/color.h"
#include "position.h"
#include "utils.h"
#include "player.h"

std::string GeneratePlayerName(){
    const std::vector<std::string> forenames = ReadAllLines(fs::path("assets/male-names.txt"));
    const std::vector<std::string> surnames = ReadAllLines(fs::path("assets/surnames-clean.txt"));

    const int num1 = rand() % forenames.size();
    const std::string firstname = forenames[num1];

    const int num2 = rand() % surnames.size();
    const std::string surname = surnames[num2];

    return fmt::format("{} {}",firstname, surname);
}

std::string GenerateTeamName(){
    const std::vector<std::string> town_file = ReadAllLines("assets/towns.txt");
    const std::vector<std::string> ex_file = ReadAllLines("assets/team_extensions.txt");

    const int num1 = rand() % town_file.size();
    const std::string town = town_file[num1];

    const int num2 = rand() % ex_file.size();
    const std::string extension = ex_file[num2];

    return town + " " + extension;
}

int GenerateDribbling(Position p){
    switch(p){
        case Position::GK: return GenerateOneNormal(25,12);
        case Position::LB: return GenerateOneNormal(60,10);
        case Position::RB: return GenerateOneNormal(60,10);
        case Position::CB: return GenerateOneNormal(40,15);
        case Position::DM: return GenerateOneNormal(50,15);
        case Position::AM: return GenerateOneNormal(65,15);
        case Position::CM: return GenerateOneNormal(60,13);
        case Position::LM: return GenerateOneNormal(70,15);
        case Position::RM: return GenerateOneNormal(70,15);
        case Position::LW: return GenerateOneNormal(72,12);
        case Position::RW: return GenerateOneNormal(72,12);
        case Position::ST: return GenerateOneNormal(65,12);
        default: return GenerateOneNormal(50,25);
    }
}

int GenerateFinishing(Position p){
    switch(p){
        case Position::GK: return GenerateOneNormal(15,5);
        case Position::LB: return GenerateOneNormal(35,10);
        case Position::RB: return GenerateOneNormal(35,10);
        case Position::CB: return GenerateOneNormal(30,15);
        case Position::DM: return GenerateOneNormal(45,15);
        case Position::AM: return GenerateOneNormal(65,25);
        case Position::CM: return GenerateOneNormal(55,15);
        case Position::LM: return GenerateOneNormal(70,10);
        case Position::RM: return GenerateOneNormal(70,10);
        case Position::LW: return GenerateOneNormal(75,10);
        case Position::RW: return GenerateOneNormal(75,10);
        case Position::ST: return GenerateOneNormal(85,5);
        default: return GenerateOneNormal(50,25);
    }
}

int GenerateGoalPrevention(Position p){
    switch(p){
        case Position::GK: return GenerateOneNormal(75,5);
        case Position::LB: return GenerateOneNormal(70,10);
        case Position::RB: return GenerateOneNormal(70,10);
        case Position::CB: return GenerateOneNormal(80,10);
        case Position::DM: return GenerateOneNormal(80,10);
        case Position::AM: return GenerateOneNormal(45,15);
        case Position::CM: return GenerateOneNormal(60,10);
        case Position::LM: return GenerateOneNormal(57,8);
        case Position::RM: return GenerateOneNormal(57,8);
        case Position::LW: return GenerateOneNormal(45,8);
        case Position::RW: return GenerateOneNormal(45,8);
        case Position::ST: return GenerateOneNormal(30,10);
        default: return GenerateOneNormal(50,25);
    }
}

int GenerateTackling(Position p){
    switch(p){
        case Position::GK: return GenerateOneNormal(70,5);
        case Position::LB: return GenerateOneNormal(70,10);
        case Position::RB: return GenerateOneNormal(70,10);
        case Position::CB: return GenerateOneNormal(80,10);
        case Position::DM: return GenerateOneNormal(80,10);
        case Position::AM: return GenerateOneNormal(50,15);
        case Position::CM: return GenerateOneNormal(60,10);
        case Position::LM: return GenerateOneNormal(60,8);
        case Position::RM: return GenerateOneNormal(60,8);
        case Position::LW: return GenerateOneNormal(45,8);
        case Position::RW: return GenerateOneNormal(45,8);
        case Position::ST: return GenerateOneNormal(35,10);
        default: return GenerateOneNormal(50,25);
    }
}

int GeneratePassing(Position p){
    return GenerateOneNormal(60,25);
}

int GenerateAssisting(Position p){
    switch(p){
        case Position::GK: return GenerateOneNormal(30,20);
        case Position::LB: return GenerateOneNormal(60,15);
        case Position::RB: return GenerateOneNormal(60,15);
        case Position::CB: return GenerateOneNormal(40,10);
        case Position::DM: return GenerateOneNormal(55,15);
        case Position::AM: return GenerateOneNormal(70,15);
        case Position::CM: return GenerateOneNormal(60,15);
        case Position::LM: return GenerateOneNormal(72,10);
        case Position::RM: return GenerateOneNormal(72,10);
        case Position::LW: return GenerateOneNormal(70,10);
        case Position::RW: return GenerateOneNormal(70,10);
        case Position::ST: return GenerateOneNormal(60,15);
        default: return GenerateOneNormal(50,25);
    }
}

Player GeneratePlayer(Position p, int number){
    Player pl = Player(
        GeneratePlayerName(), 
        number, 
        p, 
        GenerateDribbling(p),
        GenerateFinishing(p),
        GenerateTackling(p),
        GeneratePassing(p),
        GenerateAssisting(p),
        GenerateGoalPrevention(p)
    );

    return pl;
}