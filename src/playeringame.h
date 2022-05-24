#include "player.h"
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"

#ifndef PLAYERINGAME
#define PLAYERINGAME

class PlayerInGame{
    // private:
        
    public:
        // Constructor
        PlayerInGame(Player _player);

        // Attributes
        Player player; // Player referenced
        int cardValue; // Value of the card (0 = none, 1 = yellow, 2 = red)
        bool sentOff;  // Bool to store if the player is sent off
        double rating; // Rating of the player (between 0 and 10)

        // Public function headers
        void GiveCard(bool isYellow = true); // Give a card to a player of the given type
        //void GiveCard(Game& game, bool isYellow = true) // Gamelog version, TODO
};

#endif