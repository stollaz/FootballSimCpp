#include "player.h"
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"
// #include "game.cpp"

#ifndef PLAYERINGAME
#define PLAYERINGAME

class PlayerInGame{
    // private:
        
    public:
        // Constructor
        PlayerInGame(Player _player) : player(_player){
            cardValue = 0;
            sentOff = false;
            rating = 6.0;
        }

        // Attributes
        Player player; // Player referenced
        int cardValue; // Value of the card (0 = none, 1 = yellow, 2 = red)
        bool sentOff;  // Bool to store if the player is sent off
        double rating; // Rating of the player (between 0 and 10)

        // Public function headers
        void GiveCard(bool isYellow = true); // Give a card to a player of the given type
        // void GiveCard(Game& game, bool isYellow = true); // Gamelog version, TODO
        // The above will not work as it requires "game.h" to be included, but "game.h" already
        //  includes "playeringame.h" (this file), so loops infinitely
        // I have no idea if there is a solution to this, but for now cards will simply not log in eventLog
};

#endif