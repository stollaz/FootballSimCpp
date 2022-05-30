#include "player.h"
#include "generate.h"
#include "game.h"
#include "teamgamestats.h"

#include <math.h>

#define __max

PlayerInGame GetSetPieceTaker(TeamGameStats team){
    Player t = Player("",0,Position::GK,0,0,0,0,0,0);
    PlayerInGame bestPlayer = PlayerInGame(t);

    for (PlayerInGame p : team.players) if (p.player.finishing > bestPlayer.player.finishing) bestPlayer = p;

    return bestPlayer;
}

bool AttemptPass(PlayerInGame& p1, PlayerInGame& p2, PlayerInGame& p3, bool show = true){
    double passChance = Sigmoid(p1.player.passing,__max(p3.player.tackling, p3.player.goalPrevention),-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    if (show) fmt::print("> {} attempts a pass to {}...\n",p1.player.name, p2.player.name);

    if (randnum < passChance) {
        if (show) fmt::print("> The pass was successful.\n");
        return true;
    }
    else{
        if (show) fmt::print("> The pass is intercepted by {}.\n",p3.player.name);
        return false;
    }
}

bool AttemptPass(Game& game, PlayerInGame& p1, PlayerInGame& p2, PlayerInGame& p3, bool show = true){
    double passChance = Sigmoid(p1.player.passing,__max(p3.player.tackling, p3.player.goalPrevention),-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} attempts a pass to {}...",p1.player.name, p2.player.name)));

    if (randnum < passChance) {
        game.AddToLog(LogItem(ItemType::TextLine, ("> The pass was successful.")));
        return true;
    }
    else{
        game.AddToLog(LogItem(ItemType::TextLine, fmt::format("> The pass is intercepted by {}",p3.player.name)));
        return false;
    }
}

bool AttemptDribble(PlayerInGame& p1, PlayerInGame& p2, bool show = true){
    double dribbleChance = Sigmoid(p1.player.dribbling, p2.player.tackling,-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    if (randnum < dribbleChance) {
        if (show) fmt::print("> {} dribbled past {}.",p1.player.name, p2.player.name);
        return true;
    }
    else{
        if (show) fmt::print("> {} was tackled by {}.",p1.player.name, p2.player.name);
        return false;
    }
}

bool AttemptDribble(Game& game, PlayerInGame& p1, PlayerInGame& p2, bool show = true){
    double dribbleChance = Sigmoid(p1.player.dribbling, p2.player.tackling,-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    if (randnum < dribbleChance) {
        game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} dribbled past {}.",p1.player.name,p2.player.name)));
        return true;
    }
    else{
        game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} was tackled by {}.",p1.player.name,p2.player.name)));
        return false;
    }
}

bool AttemptShot(TeamGameStats& team1stats, TeamGameStats& team2stats, PlayerInGame& p1, bool isTeam1, bool show = true){
    PlayerInGame gk = isTeam1 ? team2stats.players[0] : team1stats.players[0];

    if (isTeam1) team1stats.shots++;
    else team2stats.shotAccuracy++;

    if (show){
        if (isTeam1) fmt::print("> [{}] {} attempts a shot.\n",team1stats.team.name, p1.player.name);
        else fmt::print("> [{}] {} attempts a shot.\n",team2stats.team.name, p1.player.name);
        fmt::print("> ");
    }

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);

    double hitchance = GenerateOneNormal(40,25);

    if (p1.player.finishing < hitchance){
        p1.rating-=0.3; // Decrease rating for missed shot
        if (show){
            fmt::print(fg(fmt::color::red), "MISS. ");
            fmt::print("{} missed the target.\n",p1.player.name);
        }
        return false;
    }

    if (isTeam1) team1stats.shotsOnTarget++; else team2stats.shotAccuracy++;

    double shotchance = Sigmoid(p1.player.finishing, gk.player.goalPrevention);
    double randnum = dist(gen);

    if (randnum < shotchance){
        p1.rating+=1; // Increase p1 rating for goal
        gk.rating-=0.5; // Decreate keeper rating for failed save
        team1stats.goals++;
        if (show){
            fmt::print(fg(fmt::color::green),"GOAL! ");
            if (isTeam1) fmt::print("{} scores for {}!\n",p1.player.name,team1stats.team.name);
            else fmt::print("{} scores for {}!\n",p1.player.name,team2stats.team.name);
        }
        return true;
    }
    else{
        p1.rating+=0.3; // Slightly increase rating for shot on target
        gk.rating+=0.5; // Increase keeper rating for save
        team2stats.saves++;
        if (show){
            fmt::print(fg(fmt::color::yellow),"SAVE! ");
            if (isTeam1) fmt::print("{} saves the shot for {}!\n",gk.player.name,team2stats.team.name);
            else fmt::print("{} saves the shot for {}!\n",gk.player.name,team1stats.team.name);
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

bool AttemptShot(Game& game, TeamGameStats& team1stats, TeamGameStats& team2stats, PlayerInGame& p1, bool isTeam1, bool show = true){
    PlayerInGame gk = isTeam1 ? team2stats.players[0] : team1stats.players[0];

    if (isTeam1) team1stats.shots++;
    else team2stats.shotAccuracy++;

    if (isTeam1) game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> [{}] {} attempts a shot.",team1stats.team.name, p1.player.name)));
    else game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> [{}] {} attempts a shot.",team2stats.team.name, p1.player.name)));
    game.AddToLog(LogItem(ItemType::Text,"> "));

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);

    double hitchance = GenerateOneNormal(40,25);

    if (p1.player.finishing < hitchance){
        p1.rating-=0.3; // Lower rating for missed shot
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::red), "MISS. ")));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("{} missed the target.\n",p1.player.name)));
        return false;
    }

    if (isTeam1) team1stats.shotsOnTarget++; else team2stats.shotAccuracy++;

    double shotchance = Sigmoid(p1.player.finishing, gk.player.goalPrevention);
    double randnum = dist(gen);

    if (randnum < shotchance){
        p1.rating+=1; // Increase p1 rating for goal
        if (isTeam1) team2stats.players[0].rating-=0.5; // Decrease keeper rating for failed save
        else team1stats.players[0].rating-=0.5;
        team1stats.goals++;
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::lime_green),"GOAL! ")));
        if (isTeam1) game.AddToLog(LogItem(ItemType::Text,fmt::format("{} scores for {}!\n",p1.player.name,team1stats.team.name)));
        else game.AddToLog(LogItem(ItemType::Text,fmt::format("{} scores for {}!\n",p1.player.name,team2stats.team.name)));
        // fmt::print(fg(fmt::color::green),"GOAL! ");
        // if (isTeam1) fmt::print("{} scores for {}!\n",p1.name,team1stats.team.name);
        // else fmt::print("{} scores for {}!\n",p1.name,team2stats.team.name);
        return true;
    }
    else{
        p1.rating+=0.3; // Slightly increase rating for shot on target
        if (isTeam1) team2stats.players[0].rating+=0.5; // Increase keeper rating for save
        else team1stats.players[0].rating+=0.5;
        team2stats.saves++;
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::yellow),"SAVE! ")));
        if (isTeam1) game.AddToLog(LogItem(ItemType::Text,fmt::format("{} saves the shot for {}!\n",gk.player.name,team2stats.team.name)));
        else game.AddToLog(LogItem(ItemType::Text,fmt::format("{} saves the shot for {}!\n",gk.player.name,team1stats.team.name)));
        // fmt::print(fg(fmt::color::yellow),"SAVE! ");
        // if (isTeam1) fmt::print("{} saves the shot for {}!\n",gk.name,team2stats.team.name);
        // else fmt::print("{} saves the shot for {}!\n",gk.name,team1stats.team.name);
        return false;
    }
}

bool AttemptSetPiece(TeamGameStats& team1, TeamGameStats& team2, bool isFreeKick, bool isTeam1, bool show = true){
    TeamGameStats teamTaking = isTeam1 ? team1 : team2;

    PlayerInGame bestPlayer = GetSetPieceTaker(teamTaking);

    if (show) fmt::print("> ");
    if (isFreeKick){
        if (show){
            fmt::print(fg(fmt::color::magenta), "FREE KICK! ");
            fmt::print("{} are awarded a free kick.\n",teamTaking.team.name);
            fmt::print("> [{}] {} steps up to take the free kick...\n",teamTaking.team.name,bestPlayer.player.name);
        }
    }
    else{
        if (show){
            fmt::print(fg(fmt::color::magenta), "PENALTY! ");
            fmt::print("{} are awarded a penalty.\n",teamTaking.team.name);
            fmt::print("> [{}] {} steps up to take the penalty...\n",teamTaking.team.name,bestPlayer.player.name);
        }
        bestPlayer.player.finishing += 10; // Give taking player a finishing boost to indicate penalties are 'easier'
    }

    return AttemptShot(team1, team2, bestPlayer, isTeam1, show);
}

bool AttemptSetPiece(Game& game, TeamGameStats& team1, TeamGameStats& team2, bool isFreeKick, bool isTeam1, bool show = true){
    TeamGameStats teamTaking = isTeam1 ? team1 : team2;

    PlayerInGame bestPlayer = GetSetPieceTaker(teamTaking);

    game.AddToLog(LogItem(ItemType::Text, "> "));
    if (isFreeKick){
        // game.AddToLog(LogItem(ItemType::Colour, "magenta"));
        // game.AddToLog(LogItem(ItemType::Text, "FREE KICK! "));
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::magenta), "FREE KICK! ")));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("{} are awarded a free kick.\n",teamTaking.team.name)));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("> [{}] {} steps up to take the free kick...\n",teamTaking.team.name,bestPlayer.player.name)));
        // fmt::print(fg(fmt::color::magenta), "FREE KICK! ");
        // fmt::print("{} are awarded a free kick.\n",teamTaking.name);
        // fmt::print("> [{}] {} steps up to take the free kick...\n",teamTaking.name,bestPlayer.name);
    }
    else{
        // fmt::print(fg(fmt::color::magenta), "PENALTY! ");
        // fmt::print("{} are awarded a penalty.\n",teamTaking.name);
        // fmt::print("> [{}] {} steps up to take the penalty...\n",teamTaking.name,bestPlayer.name);
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::magenta), "PENALTY! ")));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("{} are awarded a penalty.\n",teamTaking.team.name)));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("> [{}] {} steps up to take the penalty...\n",teamTaking.team.name,bestPlayer.player.name)));
        bestPlayer.player.finishing += 10; // Give taking player a finishing boost to indicate penalties are 'easier'
    }

    return AttemptShot(game, team1, team2, bestPlayer, isTeam1, show);
}

void GiveCard(PlayerInGame& p, bool isYellow = true, bool show = true){
    if (show) fmt::print("> ");
    if (isYellow){
        if (p.cardValue == 1){
            p.cardValue++;
            p.sentOff = true;
            if (show) {
                fmt::print(fg(fmt::color::red),"SECOND YELLOW! ");
                fmt::print("{} has been sent off for a 2nd bookable offense.\n",p.player.name);
            }
            p.rating-=1;
        }
        else{
            p.cardValue++;
            if (show){
                fmt::print(fg(fmt::color::yellow),"YELLOW CARD! ");
                fmt::print("{} has been booked.\n",p.player.name);
            }
            p.rating-=0.5;
        }
    }
    else{
        p.sentOff = true;
        p.cardValue = 2;
        if (show){
            fmt::print(fg(fmt::color::red),"RED CARD! ");
            fmt::print("{} has been sent off.\n",p.player.name);
        }
        p.rating-=2;
    }
}

void GiveCard(Game& game, PlayerInGame& p, bool isYellow = true, bool show = true){
    game.AddToLog(LogItem(ItemType::Text,"> "));
    if (isYellow){
        if (p.cardValue == 1){
            p.cardValue++;
            p.sentOff = true;
            game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::red),"SECOND YELLOW! ")));
            game.AddToLog(LogItem(ItemType::Text,fmt::format("{} has been sent off for a 2nd bookable offense.\n",p.player.name)));
            p.rating -=1;
        }
        else{
            p.cardValue++;
            game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::yellow),"YELLOW CARD! ")));
            game.AddToLog(LogItem(ItemType::Text,fmt::format("{} has been booked.\n",p.player.name)));
            p.rating-=0.5;
        }
    }
    else{
        p.sentOff = true;
        p.cardValue = 2;
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::red),"RED CARD! ")));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("{} has been sent off.\n",p.player.name)));
        p.rating-=2;
    }
}