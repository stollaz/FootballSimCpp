#include <string>
#include <vector>
#include "rotatableteam.h"

#ifndef GAMEWEEK
#define GAMEWEEK

class GameWeek{
    private:
    public:
        std::vector<std::string> _games;
        std::vector<std::tuple<RotatableTeam,RotatableTeam>> games;
        int weekNum;

        GameWeek(int n);
        void AddGame(std::string s);
        void AddGame(std::tuple<RotatableTeam,RotatableTeam> s);
        void Shuffle();
};

#endif