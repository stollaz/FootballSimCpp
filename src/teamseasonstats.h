#include <string>
#include "team.h"

#ifndef TEAMSEASONSTATS
#define TEAMSEASONSTATS

class TeamSeasonStats{
    private:
    public:
        Team team;
        int wins;
        int draws;
        int losses;
        int goalsFor;
        int goalsAgainst;
        int points;
        int goalDiff;

        void CalculatePoints();
        void CalculateGoalDiff();
        TeamSeasonStats(Team _team);
};

#endif