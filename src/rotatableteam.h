#include <string>
#include "player.h"
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"
#include <vector>

#ifndef ROTATABLETEAM
#define ROTATABLETEAM

class RotatableTeam{
    private:
        Player null_player = Player("null",0,Position::GK,0,0,0,0,0,0); // Dummy null player used as placeholder

    public:
        // Constructors
        RotatableTeam();
        RotatableTeam(std::string _name);
        
        // Attributes
        std::string name;            // Name of the team
        std::vector<Player> players; // Variable length vector of players in the team
        Player bestXI[11];           // Array of best XI players in the team
        int rating;                  // Average rating of the team

        // Public function headers
        void AddPlayer(Player p); // Method to add player to the team
        void InitialiseTeam();    // Method to initialise the team with null players
        void CalculateRating();   // Method to calculate the average rating for the team
        void GenerateBestXI();    // Method to generate the best XI from a given vector of players
        Player GetBestPlayer(Position Pos, std::vector<Player> OrderedList);  // Function to return the best player in a given position
        Player _GetBestPlayer(Position Pos, std::vector<Player> OrderedList); // Unused
        void SaveTeam();          // Method to save the team to a file
};

#endif