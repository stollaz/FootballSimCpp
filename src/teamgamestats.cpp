#include "teamgamestats.h"

void TeamGameStats::CalculateShotAccuracy(){
    if (shots == 0) shotAccuracy = 0;
    else shotAccuracy = (shotsOnTarget*100)/shots;
}

void TeamGameStats::CalculatePassAccuracy(){
    if (passesAttempted == 0) passAccuracy = 0;
    else passAccuracy = (passes*100)/passesAttempted;
}

void TeamGameStats::SetPosession(int p){
    if (p > 100) posession = 100;
    else if (p < 0) posession = 0;
    else posession = p;
}

// TeamGameStats::TeamGameStats(Team _team){
//     fmt::print("ASDHSUADIHUASIDHUDAS\n");
//     team = _team;
//     shots = 0;
//     shotsOnTarget = 0;
//     goals = 0;
//     saves = 0;
//     posession = 0;
//     fouls = 0;
//     yellowCards = 0;
//     redCards = 0;
//     passesAttempted = 0;
//     passes = 0;
//     interceptions = 0;
//     tackles = 0;
//     shotAccuracy = 0;
//     passAccuracy = 0;
// }