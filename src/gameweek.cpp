#include "gameweek.h"
#include <algorithm>
#include <random>

GameWeek::GameWeek(int n){
    weekNum = n;
}

void GameWeek::AddGame(std::string s){
    _games.push_back(s);
}

void GameWeek::AddGame(std::tuple<RotatableTeam,RotatableTeam> s){
    games.push_back(s);
}

void GameWeek::Shuffle(){
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(games), std::end(games), rng);
}