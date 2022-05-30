#include "player.h"
#include "generate.h"
#include "game.h"
#include "teamgamestats.h"

#include <math.h>

#define __max

Player GetSetPieceTaker(Team team){
    Player bestPlayer = Player("",0,Position::GK,0,0,0,0,0,0);

    for (Player p : team.players) if (p.finishing > bestPlayer.finishing) bestPlayer = p;

    return bestPlayer;
}

bool AttemptPass(Player p1, Player p2, Player p3, bool show = true){
    double passChance = Sigmoid(p1.passing,__max(p3.tackling, p3.goalPrevention),-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    if (show) fmt::print("> {} attempts a pass to {}...\n",p1.name, p2.name);

    if (randnum < passChance) {
        if (show) fmt::print("> The pass was successful.\n");
        return true;
    }
    else{
        if (show) fmt::print("> The pass is intercepted by {}.\n",p3.name);
        return false;
    }
}

bool AttemptPass(Game& game, Player p1, Player p2, Player p3, bool show = true){
    double passChance = Sigmoid(p1.passing,__max(p3.tackling, p3.goalPrevention),-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} attempts a pass to {}...",p1.name, p2.name)));

    if (randnum < passChance) {
        game.AddToLog(LogItem(ItemType::TextLine, ("> The pass was successful.")));
        return true;
    }
    else{
        game.AddToLog(LogItem(ItemType::TextLine, fmt::format("> The pass is intercepted by {}",p3.name)));
        return false;
    }
}

bool AttemptDribble(Player p1, Player p2, bool show = true){
    double dribbleChance = Sigmoid(p1.dribbling,p2.tackling,-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    if (randnum < dribbleChance) {
        if (show) fmt::print("> {} dribbled past {}.",p1.name,p2.name);
        return true;
    }
    else{
        if (show) fmt::print("> {} was tackled by {}.",p1.name,p2.name);
        return false;
    }
}

bool AttemptDribble(Game& game, Player p1, Player p2, bool show = true){
    double dribbleChance = Sigmoid(p1.dribbling,p2.tackling,-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    if (randnum < dribbleChance) {
        game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} dribbled past {}.",p1.name,p2.name)));
        return true;
    }
    else{
        game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} was tackled by {}.",p1.name,p2.name)));
        return false;
    }
}

bool AttemptShot(TeamGameStats team1stats, TeamGameStats team2stats, Player p1, bool isTeam1, bool show = true){
    Player gk = isTeam1 ? team2stats.team.players[0] : team1stats.team.players[0];

    if (isTeam1) team1stats.shots++;
    else team2stats.shotAccuracy++;

    if (show){
        if (isTeam1) fmt::print("> [{}] {} attempts a shot.\n",team1stats.team.name, p1.name);
        else fmt::print("> [{}] {} attempts a shot.\n",team2stats.team.name, p1.name);
        fmt::print("> ");
    }

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);

    double hitchance = GenerateOneNormal(40,25);

    if (p1.finishing < hitchance){
        //TODO // Lower rating for missed shot
        if (show){
            fmt::print(fg(fmt::color::red), "MISS. ");
            fmt::print("{} missed the target.\n",p1.name);
        }
        return false;
    }

    if (isTeam1) team1stats.shotsOnTarget++; else team2stats.shotAccuracy++;

    double shotchance = Sigmoid(p1.finishing, gk.goalPrevention);
    double randnum = dist(gen);

    if (randnum < shotchance){
        //TODO // Increase p1 rating for goal
        //TODO // Decreate keeper rating for failed save
        team1stats.goals++;
        if (show){
            fmt::print(fg(fmt::color::green),"GOAL! ");
            if (isTeam1) fmt::print("{} scores for {}!\n",p1.name,team1stats.team.name);
            else fmt::print("{} scores for {}!\n",p1.name,team2stats.team.name);
        }
        return true;
    }
    else{
        //TODO // Slightly increase rating for shot on target
        //TODO // Increase keeper rating for save
        team2stats.saves++;
        if (show){
            fmt::print(fg(fmt::color::yellow),"SAVE! ");
            if (isTeam1) fmt::print("{} saves the shot for {}!\n",gk.name,team2stats.team.name);
            else fmt::print("{} saves the shot for {}!\n",gk.name,team1stats.team.name);
        }
        return false;
    }

    // if (isTeam1){
    //     team1stats.shotsOnTarget++;

    //     double shotchance = Sigmoid(p1.finishing, gk.goalPrevention);
    //     double randnum = dist(gen);

    //     if (randnum < shotchance){
    //         //TODO // Increase p1 rating for goal
    //         //TODO // Decreate keeper rating for failed save
    //         team1stats.goals++;
    //         if (show){
    //             fmt::print(fg(fmt::color::green),"GOAL! ");
    //             fmt::print("{} scores for {}!\n",p1.name,team1stats.team.name);
    //         }
    //         return true;
    //     }
    //     else{
    //         //TODO // Slightly increase rating for shot on target
    //         //TODO // Increase keeper rating for save
    //         team2stats.saves++;
    //         if (show){
    //             fmt::print(fg(fmt::color::yellow),"GOAL! ");
    //             fmt::print("{} saves the shot for {}!\n",gk.name,team2stats.team.name);
    //         }
    //         return false;
    //     }
    // }
}

bool AttemptShot(Game& game, TeamGameStats team1stats, TeamGameStats team2stats, Player p1, bool isTeam1, bool show = true){
    Player gk = isTeam1 ? team2stats.team.players[0] : team1stats.team.players[0];

    if (isTeam1) team1stats.shots++;
    else team2stats.shotAccuracy++;

    if (isTeam1) game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> [{}] {} attempts a shot.",team1stats.team.name, p1.name)));
    else game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> [{}] {} attempts a shot.",team2stats.team.name, p1.name)));
    game.AddToLog(LogItem(ItemType::Text,"> "));
    // if (isTeam1) fmt::print("> [{}] {} attempts a shot.\n",team1stats.team.name, p1.name);
    // else fmt::print("> [{}] {} attempts a shot.\n",team2stats.team.name, p1.name);
    // fmt::print("> ");

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);

    double hitchance = GenerateOneNormal(40,25);

    if (p1.finishing < hitchance){
        //TODO // Lower rating for missed shot
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::red), "MISS. ")));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("{} missed the target.\n",p1.name)));
        // fmt::print(fg(fmt::color::red), "MISS. ");
        // fmt::print("{} missed the target.\n",p1.name);
        return false;
    }

    if (isTeam1) team1stats.shotsOnTarget++; else team2stats.shotAccuracy++;

    double shotchance = Sigmoid(p1.finishing, gk.goalPrevention);
    double randnum = dist(gen);

    if (randnum < shotchance){
        //TODO // Increase p1 rating for goal
        //TODO // Decreate keeper rating for failed save
        team1stats.goals++;
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::lime_green),"GOAL! ")));
        if (isTeam1) game.AddToLog(LogItem(ItemType::Text,fmt::format("{} scores for {}!\n",p1.name,team1stats.team.name)));
        else game.AddToLog(LogItem(ItemType::Text,fmt::format("{} scores for {}!\n",p1.name,team2stats.team.name)));
        // fmt::print(fg(fmt::color::green),"GOAL! ");
        // if (isTeam1) fmt::print("{} scores for {}!\n",p1.name,team1stats.team.name);
        // else fmt::print("{} scores for {}!\n",p1.name,team2stats.team.name);
        return true;
    }
    else{
        //TODO // Slightly increase rating for shot on target
        //TODO // Increase keeper rating for save
        team2stats.saves++;
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::yellow),"SAVE! ")));
        if (isTeam1) game.AddToLog(LogItem(ItemType::Text,fmt::format("{} saves the shot for {}!\n",gk.name,team2stats.team.name)));
        else game.AddToLog(LogItem(ItemType::Text,fmt::format("{} saves the shot for {}!\n",gk.name,team1stats.team.name)));
        // fmt::print(fg(fmt::color::yellow),"SAVE! ");
        // if (isTeam1) fmt::print("{} saves the shot for {}!\n",gk.name,team2stats.team.name);
        // else fmt::print("{} saves the shot for {}!\n",gk.name,team1stats.team.name);
        return false;
    }
}

bool AttemptSetPiece(TeamGameStats team1, TeamGameStats team2, bool isFreeKick, bool isTeam1, bool show = true){
    Team teamTaking = isTeam1 ? team1.team : team2.team;

    Player bestPlayer = GetSetPieceTaker(teamTaking);

    if (show) fmt::print("> ");
    if (isFreeKick){
        if (show){
            fmt::print(fg(fmt::color::magenta), "FREE KICK! ");
            fmt::print("{} are awarded a free kick.\n",teamTaking.name);
            fmt::print("> [{}] {} steps up to take the free kick...\n",teamTaking.name,bestPlayer.name);
        }
    }
    else{
        if (show){
            fmt::print(fg(fmt::color::magenta), "PENALTY! ");
            fmt::print("{} are awarded a penalty.\n",teamTaking.name);
            fmt::print("> [{}] {} steps up to take the penalty...\n",teamTaking.name,bestPlayer.name);
        }
        bestPlayer.finishing += 10; // Give taking player a finishing boost to indicate penalties are 'easier'
    }

    return AttemptShot(team1, team2, bestPlayer, isTeam1, show);
}

bool AttemptSetPiece(Game& game, TeamGameStats team1, TeamGameStats team2, bool isFreeKick, bool isTeam1, bool show = true){
    Team teamTaking = isTeam1 ? team1.team : team2.team;

    Player bestPlayer = GetSetPieceTaker(teamTaking);

    game.AddToLog(LogItem(ItemType::Text, "> "));
    if (isFreeKick){
        // game.AddToLog(LogItem(ItemType::Colour, "magenta"));
        // game.AddToLog(LogItem(ItemType::Text, "FREE KICK! "));
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::magenta), "FREE KICK! ")));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("{} are awarded a free kick.\n",teamTaking.name)));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("> [{}] {} steps up to take the free kick...\n",teamTaking.name,bestPlayer.name)));
        // fmt::print(fg(fmt::color::magenta), "FREE KICK! ");
        // fmt::print("{} are awarded a free kick.\n",teamTaking.name);
        // fmt::print("> [{}] {} steps up to take the free kick...\n",teamTaking.name,bestPlayer.name);
    }
    else{
        // fmt::print(fg(fmt::color::magenta), "PENALTY! ");
        // fmt::print("{} are awarded a penalty.\n",teamTaking.name);
        // fmt::print("> [{}] {} steps up to take the penalty...\n",teamTaking.name,bestPlayer.name);
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::magenta), "PENALTY! ")));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("{} are awarded a penalty.\n",teamTaking.name)));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("> [{}] {} steps up to take the penalty...\n",teamTaking.name,bestPlayer.name)));
        bestPlayer.finishing += 10; // Give taking player a finishing boost to indicate penalties are 'easier'
    }

    return AttemptShot(game, team1, team2, bestPlayer, isTeam1, show);
}

