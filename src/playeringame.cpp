#include "playeringame.h"

// PlayerInGame::PlayerInGame(Player _player){
//     PlayerInGame::player = _player;
//     PlayerInGame::cardValue = 0;
//     PlayerInGame::sentOff = false;
//     PlayerInGame::rating = 6.0;
// }

// Method to give a player a card of the specified colour, and handle sending offs
void PlayerInGame::GiveCard(bool isYellow){
    fmt::print("> ");
    if (isYellow){
        if (PlayerInGame::cardValue == 1){
            PlayerInGame::cardValue++;
            PlayerInGame::sentOff = true;
            // fmt::color c = GetColour(x);
            //  fmt::print(fg(c),"{}\n", x);
            fmt::print(fg(fmt::color::red),"SECOND YELLOW! ");
            fmt::print("{} has been sent off for a 2nd bookable offense.\n",PlayerInGame::player.name);
        }
        else{
            PlayerInGame::cardValue++;
            fmt::print(fg(fmt::color::yellow),"YELLOW CARD! ");
            fmt::print("{} has been booked.\n",PlayerInGame::player.name);
        }
    }
    else{
        PlayerInGame::sentOff = true;
        PlayerInGame::cardValue = 2;
        fmt::print(fg(fmt::color::red),"RED CARD! ");
        fmt::print("{} has been sent off.\n",PlayerInGame::player.name);
    }
}

// Method to give a player a card and add it to the game log (TODO)
// void PlayerInGame::GiveCard(Game& game, bool isYellow){
//     game.AddToLog(LogItem(ItemType::Text, "> "));
//     if (isYellow){
//         if (cardValue == 1){
//             cardValue++;
//             sentOff = true;
//         }
//     }
// }