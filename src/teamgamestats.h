#include <string>
#include "team.h"

#ifndef TEAMGAMESTATS
#define TEAMGAMESTATS

class TeamGameStats{
    private:
    public:
        Team team;
        int shots;
        int shotsOnTarget;
        int goals;
        int saves;
        int posession;
        int fouls;
        int yellowCards;
        int redCards;
        int passesAttempted;
        int passes;
        int interceptions;
        int tackles;
        int shotAccuracy;
        int passAccuracy;

        void CalculateShotAccuracy();
        void CalculatePassAccuracy();
        void SetPosession(int p);
        TeamGameStats();
        TeamGameStats(Team _team);
};

#endif