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

        Game(std::string _id, Team _team1, Team _team2, TeamGameStats _t1s, TeamGameStats _t2s) : 
        id(_id), team1(_team1), team2(_team2), team1stats(_t1s), team2stats(_t2s){}
        void AddToLog(LogItem s);
        void AddAndPrint(LogItem s);
        void PrintLog();

        void AddText(std::string s);
        void AddTextLine(std::string s);
        void AddRead();
        void AddBlankLine();
};

#endif