#include "game.h"

void Game::AddToLog(LogItem s){
    m_eventLog.push_back(s);
}

void Game::AddAndPrint(LogItem s){
    m_eventLog.push_back(s);
    if (s.m_itemType == ItemType::Text) fmt::print("{}",s.m_itemValue);
    else if (s.m_itemType == ItemType::TextLine) fmt::print("{}\n",s.m_itemValue);
}

void Game::PrintLog(){
    for (LogItem item : m_eventLog){
        if (item.m_itemType == ItemType::Text) fmt::print("{}",item.m_itemValue);
        else if (item.m_itemType == ItemType::TextLine) fmt::print("{}\n",item.m_itemValue);
        else if (item.m_itemType == ItemType::Read) system("pause");
    }
}

void Game::AddText(std::string s){
    m_eventLog.push_back(LogItem(ItemType::Text,s));
}

void Game::AddTextLine(std::string s){
    m_eventLog.push_back(LogItem(ItemType::TextLine,s));
}

void Game::AddRead(){
    m_eventLog.push_back(LogItem(ItemType::Read,""));
}

void Game::AddBlankLine(){
    m_eventLog.push_back(LogItem(ItemType::TextLine,""));
}