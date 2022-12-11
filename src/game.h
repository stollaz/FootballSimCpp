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
    public:
        Game(std::string _id, Team _team1, Team _team2, TeamGameStats _t1s, TeamGameStats _t2s) : 
            m_id(_id), m_team1(_team1), m_team2(_team2), m_team1stats(_t1s), m_team2stats(_t2s){}
        
        void AddToLog(LogItem s);
        void AddAndPrint(LogItem s);
        void PrintLog();

        void AddText(std::string s);
        void AddTextLine(std::string s);
        void AddRead();
        void AddBlankLine();
    
        std::string m_id;
        Team m_team1;
        Team m_team2;
        TeamGameStats m_team1stats;
        TeamGameStats m_team2stats;
        std::vector<PlayerInGame> m_team1Players;
        std::vector<PlayerInGame> m_team2Players;
        std::vector<int> m_score;
        std::vector<LogItem> m_eventLog;
        std::vector<Goal> m_goals;
    private:
};

#endif