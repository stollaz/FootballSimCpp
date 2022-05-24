#include <string>
#include "player.h"
#include "rotatableteam.h"
#include "team.h"

#ifndef PLAYERSEASONSTATS
#define PLAYERSEASONSTATS

class PlayerSeasonStats{
    private:
    public:
        Player player;
        RotatableTeam team;
        int gamesPlayed;
        std::vector<float> ratings;
        float avgRating;
        int goalsScored;
        int assists;
        int yellowCards;
        int redCards;

        void AddRating(float r);
        void CalculateAvgRating();
        PlayerSeasonStats(Player p, RotatableTeam t);
        PlayerSeasonStats(Player p, Team t);
};

#endif