#include "teamseasonstats.h"

void TeamSeasonStats::CalculatePoints(){
    TeamSeasonStats::points = wins*3 + draws;
}
void TeamSeasonStats::CalculateGoalDiff(){
    TeamSeasonStats::goalDiff = goalsFor - goalsAgainst;
}

TeamSeasonStats::TeamSeasonStats(Team _team){
    TeamSeasonStats::team = _team;
}