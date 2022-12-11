#include "teamseasonstats.h"

void TeamSeasonStats::CalculatePoints(){
    points = wins*3 + draws;
}
void TeamSeasonStats::CalculateGoalDiff(){
    goalDiff = goalsFor - goalsAgainst;
}

TeamSeasonStats::TeamSeasonStats(Team _team){
    team = _team;
}