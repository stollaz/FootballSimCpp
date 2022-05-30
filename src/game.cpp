#include "game.h"

void Game::AddToLog(LogItem s){
    eventLog.push_back(s);
}

void Game::AddAndPrint(LogItem s){
    eventLog.push_back(s);
    if (s.itemType == ItemType::Text) fmt::print("{}",s.itemValue);
    else if (s.itemType == ItemType::TextLine) fmt::print("{}\n",s.itemValue);
}

void Game::PrintLog(){
    for (LogItem item : eventLog){
        if (item.itemType == ItemType::Text) fmt::print("{}",item.itemValue);
        else if (item.itemType == ItemType::TextLine) fmt::print("{}\n",item.itemValue);
        else if (item.itemType == ItemType::Read) system("pause");
    }
}