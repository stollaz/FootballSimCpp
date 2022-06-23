#include <string>
#include "team.h"

#ifndef TEAMGAMESTATS
#define TEAMGAMESTATS

class TeamGameStats{
    private:
    public:
        Team team;
        std::vector<PlayerInGame> players;
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
        TeamGameStats(Team _team) : team{_team}{
            shots = 0;
            shotsOnTarget = 0;
            goals = 0;
            saves = 0;
            posession = 50;
            fouls = 0;
            yellowCards = 0;
            redCards = 0;
            passesAttempted = 0;
            passes = 0;
            interceptions = 0;
            tackles = 0;
            shotAccuracy = 0;
            passAccuracy = 0;
        }
};

#endif