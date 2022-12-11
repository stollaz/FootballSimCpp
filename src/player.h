#include <string>
#include "position.h"
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"

#ifndef PLAYER
#define PLAYER

class Player{
    private:
        // Function to calculate the overall of the player (naive approach)
        int CalculateOverall();

        // Very crude method to calculate positional overall
        int CalculatePositionalOverall();

        // Ensure value is constrained between 0 and 99
        int Constrain(int x);

        void AssignValues(std::string _name, int _number, Position _position, int _dribbling, int _finishing, int _tackling, int _passing, int _assisting, int _goalPrevention);
    public:
        // Constructors
        Player() = default;
        Player(std::string _name, int _number, Position _position, int _dribbling, int _finishing, int _tackling, int _passing, int _assisting, int _goalPrevention);
        Player(std::string _name, int _number, Position _position, Position _position2, int _dribbling, int _finishing, int _tackling, int _passing, int _assisting, int _goalPrevention);

        // Public function headers
        Position PositionType(Position p);
        std::string ToString();
        fmt::color GetColour(int x);
        void PrintWithColour(int x);
        void DisplayStats();
        std::string CompactRepresentation();
        std::string PrintingRepresentation();
        std::string PrintPosition(Position p);    
        bool Equals(Player p);

        // Attributes
        std::string name;
        int number;
        Position position;
        Position position2; // Secondary position
        int dribbling;      // How likely they are to complete a dribble
        int finishing;      // How likely they are to score from a shot
        int tackling;       // How likely they are to dispossess an opponent
        int goalPrevention; // How likely they are to prevent a goal
        int passing;        // How likely they are to complete a pass
        int assisting;      // How likely they are to set up a shot / goal from a pass
        int mentality;      // How likely they are to make the correct decision, or to avoid a foul (UNUSED)
        int overall;
        int positionalOverall;
        // PlayerInGame inGameStats;

        friend bool operator== (Player & lhs, Player & rhs );
};

#endif