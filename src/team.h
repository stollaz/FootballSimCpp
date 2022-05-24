#include <string>
#include "player.h"
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"

#ifndef TEAM
#define TEAM

class Team{
    private:
        Player null_player = Player("null",0,Position::GK,0,0,0,0,0,0); // Dummy null player used as placeholder

    public:
        // Constructors
        Team();
        Team(std::string _name);
        
        // Attributes
        std::string name;   // Name of the team
        Player players[11]; // Array of players in the team
        int rating;         // Average rating of the team

        // Public function headers
        void AddPlayer(Player p); // Method to add player to the team
        void InitialiseTeam();    // Method to initialise the team with null players
        void CalculateRating();   // Method to calculate the average rating for the team
        void SaveTeam();          // Method to save the team to a file
};

#endif