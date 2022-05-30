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

void Game::AddText(std::string s){
    eventLog.push_back(LogItem(ItemType::Text,s));
}

void Game::AddTextLine(std::string s){
    eventLog.push_back(LogItem(ItemType::TextLine,s));
}

void Game::AddRead(){
    eventLog.push_back(LogItem(ItemType::Read,""));
}

void Game::AddBlankLine(){
    eventLog.push_back(LogItem(ItemType::TextLine,""));
}