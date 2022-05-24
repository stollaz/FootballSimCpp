#include <string>
#include <vector>
#include "team.h"
#include "teamgamestats.h"
#include "playeringame.h"
#include "goal.h"
#include "logitem.h"

#ifndef GAME
#define GAME

class Game{
    private:
    public:
        std::string id;
        Team team1;
        Team team2;
        TeamGameStats team1stats;
        TeamGameStats team2stats;
        std::vector<PlayerInGame> team1Players;
        std::vector<PlayerInGame> team2Players;
        std::vector<int> score;
        std::vector<LogItem> eventLog;
        std::vector<Goal> goals;

        Game(std::string _id);
        void AddToLog(LogItem s);
        void AddAndPrint(LogItem s);
};

#endif