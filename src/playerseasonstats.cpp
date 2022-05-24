#include "playerseasonstats.h"

PlayerSeasonStats::PlayerSeasonStats(Player p, RotatableTeam t){
    player = p;
    team = t;
    gamesPlayed = 0;
    avgRating = 0;
    goalsScored = 0;
    assists = 0;
    yellowCards = 0;
    redCards = 0;
}

PlayerSeasonStats::PlayerSeasonStats(Player p, Team t){
    player = p;

    RotatableTeam tx = RotatableTeam(t.name);
    for (int i = 0; i < 11; i++){
        tx.bestXI[i] = t.players[i];
        tx.players[i] = t.players[i];
    }
    tx.rating = t.rating;
    team = tx;

    gamesPlayed = 0;
    avgRating = 0;
    goalsScored = 0;
    assists = 0;
    yellowCards = 0;
    redCards = 0;
}

void PlayerSeasonStats::AddRating(float r){
    ratings.push_back(r);
}

void PlayerSeasonStats::CalculateAvgRating(){ 
    avgRating = ratings.size()/gamesPlayed;
}