#include "player.h"

Player::Player(){}

// Constructor for player with 1 position
Player::Player(std::string _name, int _number, Position _position, int _dribbling, int _finishing, int _tackling, int _passing, int _assisting, int _goalPrevention){
    Player::name = _name;
    Player::number = _number;
    Player::position = _position;
    Player::dribbling = Constrain(_dribbling);
    Player::finishing = Constrain(_finishing);
    Player::tackling = Constrain(_tackling);
    Player::passing = Constrain(_passing);
    Player::assisting = Constrain(_assisting);
    Player::goalPrevention = Constrain(_goalPrevention);
    Player::mentality = 100-Player::tackling;
    overall = CalculateOverall();
    positionalOverall = CalculatePositionalOverall();
}

// Constructor for player with 2 positions
Player::Player(std::string _name, int _number, Position _position, Position _position2, int _dribbling, int _finishing, int _tackling, int _passing, int _assisting, int _goalPrevention){
    Player::name = _name;
    Player::number = _number;
    Player::position = _position;
    Player::position2 = _position2;
    Player::dribbling = Constrain(_dribbling);
    Player::finishing = Constrain(_finishing);
    Player::tackling = Constrain(_tackling);
    Player::passing = Constrain(_passing);
    Player::assisting = Constrain(_assisting);
    Player::goalPrevention = Constrain(_goalPrevention);
    Player::mentality = 100-Player::tackling;
    overall = CalculateOverall();
    positionalOverall = CalculatePositionalOverall();
}

// Returns the type of position the input is (either GK, Defender, Midfielder or Forward)
Position Player::PositionType(Position p){
    switch(p){
        case Position::GK: return Position::GK;
        case Position::DF: return Position::DF;
        case Position::LB: return Position::DF;
        case Position::RB: return Position::DF;
        case Position::CB: return Position::DF;
        case Position::MF: return Position::MF;
        case Position::CM: return Position::MF;
        case Position::LM: return Position::MF;
        case Position::RM: return Position::MF;
        case Position::AM: return Position::MF;
        case Position::DM: return Position::MF;
        case Position::FW: return Position::FW;
        case Position::ST: return Position::FW;
        case Position::LW: return Position::FW;
        case Position::RW: return Position::FW;
        default: return p;
    }
}

// Basic representation of player as a string
std::string Player::ToString(){
    // return "[" << number << "][" << position << "]" << name;
    return fmt::format("[{0}][{1}] {2}",number,position_str[position],name);
}

// Returns colour to be used for given value range
fmt::color Player::GetColour(int x){
    if (x >= 90) return fmt::color::turquoise;
    else if (x >= 75) return fmt::color::lime_green;
    else if (x >= 50) return fmt::color::yellow;
    else if (x >= 30) return fmt::color::red;
    else return fmt::color::dark_red;
}

// Prints the given number with its corresponding colour
void Player::PrintWithColour(int x){
    fmt::color c = GetColour(x);
    fmt::print(fg(c),"{}\n", x);
}

// Overview of player's stats with colours
void Player::DisplayStats(){
    fmt::print("[{}] {}, {}\n",number, name, PrintPosition(position));
    fmt::print("DRB: ");
    PrintWithColour(dribbling);
    fmt::print("FIN: ");
    PrintWithColour(finishing);
    fmt::print("TCK: ");
    PrintWithColour(tackling);
    fmt::print("PAS: ");
    PrintWithColour(passing);
    fmt::print("AST: ");
    PrintWithColour(assisting);
    fmt::print("GPV: ");
    PrintWithColour(goalPrevention);
    fmt::print("OVR: ");
    PrintWithColour(overall);
    fmt::print("---\n");
}

// Compact representation of player for storing
std::string Player::CompactRepresentation(){
    return fmt::format("{},{},{},{},{},{},{},{},{}",name,number,position_str[position],dribbling,finishing,tackling,passing,assisting,goalPrevention);
}

// Compact representation of player for printing to the screen
std::string Player::PrintingRepresentation(){
    return fmt::format("{},{},{},{},{},{},{},{},{}",name,number,position,dribbling,finishing,tackling,passing,assisting,goalPrevention);
}

// Returns text version of the given position
std::string Player::PrintPosition(Position p){
    switch(p){
        case Position::GK:
            return "Goalkeeper";
        case Position::LB:
            return "Left Back";
        case Position::CB:
            return "Centre Back";
        case Position::RB:
            return "Right Back";
        case Position::DM:
            return "Defensive Midfielder";
        case Position::CM:
            return "Central Midfielder";
        case Position::AM:
            return "Attacking Midfielder";
        case Position::LM:
            return "Left Midfielder";
        case Position::RM:
            return "Right Midfielder";
        case Position::RW:
            return "Right Winger";
        case Position::LW:
            return "Left Winger";
        case Position::ST:
            return "Striker";
        default:
            return "None";
    }
}

bool Player::Equals(Player p){
    return (name == p.name && number == p.number && position == p.position && dribbling == p.dribbling && finishing == p.finishing && tackling == p.tackling && goalPrevention == p.goalPrevention && passing == p.passing && assisting == p.assisting);
}

bool operator== ( Player & lhs, Player & rhs )
{
   return (lhs.name == rhs.name && lhs.number == rhs.number && lhs.position == rhs.position && lhs.dribbling == rhs.dribbling && 
   lhs.finishing == rhs.finishing && lhs.tackling == rhs.tackling && lhs.goalPrevention == rhs.goalPrevention && 
   lhs.passing == rhs.passing && lhs.assisting == rhs.assisting);
}