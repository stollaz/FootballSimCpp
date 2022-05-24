#include "game.h"

Game::Game(std::string _id){
    id = _id;
}

void Game::AddToLog(LogItem s){
    eventLog.push_back(s);
}

void Game::AddAndPrint(LogItem s){
    eventLog.push_back(s);
    if (s.itemType == ItemType::Text) fmt::print("{}",s.itemValue);
}