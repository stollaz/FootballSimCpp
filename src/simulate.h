#include "player.h"
#include "generate.h"
#include "game.h"
#include "teamgamestats.h"

#include <math.h>
#include <algorithm>

PlayerInGame GetSetPieceTaker(TeamGameStats team){
    Player t = Player("",0,Position::GK,0,0,0,0,0,0);
    PlayerInGame bestPlayer = PlayerInGame(t);

    for (PlayerInGame p : team.players) if (p.player.finishing > bestPlayer.player.finishing) bestPlayer = p;

    return bestPlayer;
}

void ShowTeam1Scored(Team team1, Team team2, int team1_score, int team2_score){
    fmt::print("> {} [", team1.name);
    fmt::print(fg(fmt::color::green), "{}",team1_score);
    fmt::print("] - {} {}\n",team2_score, team2.name);
}

void ShowTeam1Scored(Game& game, Team team1, Team team2, int team1_score, int team2_score){
    game.AddToLog(LogItem(ItemType::Text, fmt::format("> {} [", team1.name)));
    game.AddToLog(LogItem(ItemType::Text, fmt::format(fg(fmt::color::green), "{}",team1_score)));
    game.AddToLog(LogItem(ItemType::Text, fmt::format("] - {} {}\n",team2_score, team2.name)));
}

void ShowTeam2Scored(Team team1, Team team2, int team1_score, int team2_score){
    fmt::print("> {} {} - [", team1.name, team1_score);
    fmt::print(fg(fmt::color::green), "{}",team2_score);
    fmt::print("] {}\n", team2.name);
}

void ShowTeam2Scored(Game& game, Team team1, Team team2, int team1_score, int team2_score){
    game.AddToLog(LogItem(ItemType::Text, fmt::format("> {} {} - [", team1.name, team1_score)));
    game.AddToLog(LogItem(ItemType::Text, fmt::format(fg(fmt::color::green), "{}",team2_score)));
    game.AddToLog(LogItem(ItemType::Text, fmt::format("] {}\n", team2.name)));
}

bool AttemptPass(PlayerInGame& passer, PlayerInGame& reciever, PlayerInGame& intercepter, bool show = true){
    double passChance = Sigmoid(passer.player.passing,std::max(intercepter.player.tackling, intercepter.player.goalPrevention),-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    // if (show) fmt::print("> {} attempts a pass to {}...\n",passer.player.name, reciever.player.name);

    if (randnum < passChance) {
        if (show) fmt::print("> The pass was successful.\n");
        return true;
    }
    else{
        if (show) fmt::print("> The pass is intercepted by {}.\n",intercepter.player.name);
        return false;
    }
}

bool AttemptPass(Game& game, PlayerInGame& passer, PlayerInGame& reciever, PlayerInGame& intercepter, bool show = true){
    double passChance = Sigmoid(passer.player.passing,std::max(intercepter.player.tackling, intercepter.player.goalPrevention),-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} attempts a pass to {}...",passer.player.name, reciever.player.name)));

    if (randnum < passChance) {
        game.AddToLog(LogItem(ItemType::TextLine, ("> The pass was successful.")));
        return true;
    }
    else{
        game.AddToLog(LogItem(ItemType::TextLine, fmt::format("> The pass is intercepted by {}",intercepter.player.name)));
        return false;
    }
}

bool AttemptDribble(PlayerInGame& attacker, PlayerInGame& defender, bool show = true){
    double dribbleChance = Sigmoid(attacker.player.dribbling, defender.player.tackling,-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    if (randnum < dribbleChance) {
        if (show) fmt::print("> {} dribbled past {}.\n",attacker.player.name, defender.player.name);
        return true;
    }
    else{
        if (show) fmt::print("> {} was tackled by {}.\n",attacker.player.name, defender.player.name);
        return false;
    }
}

bool AttemptDribble(Game& game, PlayerInGame& attacker, PlayerInGame& defender, bool show = true){
    double dribbleChance = Sigmoid(attacker.player.dribbling, defender.player.tackling,-0.03,-0.5);

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);
    double randnum = dist(gen);

    if (randnum < dribbleChance) {
        game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} dribbled past {}.",attacker.player.name,defender.player.name)));
        return true;
    }
    else{
        game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> {} was tackled by {}.",attacker.player.name,defender.player.name)));
        return false;
    }
}

bool AttemptShot(TeamGameStats& team1stats, TeamGameStats& team2stats, PlayerInGame& attacker, bool isTeam1, bool show = true){
    PlayerInGame gk = isTeam1 ? team2stats.players.at(0) : team1stats.players.at(0);

    if (isTeam1) team1stats.shots++;
    else team2stats.shots++;

    if (show){
        if (isTeam1) fmt::print("> [{}] {} attempts a shot.\n",team1stats.team.name, attacker.player.name);
        else fmt::print("> [{}] {} attempts a shot.\n",team2stats.team.name, attacker.player.name);
        fmt::print("> ");
    }

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);

    double hitchance = GenerateOneNormal(40,25);

    if (attacker.player.finishing < hitchance){
        attacker.rating-=0.2; // Decrease rating for missed shot
        if (show){
            fmt::print(fg(fmt::color::red), "MISS. ");
            fmt::print("{} missed the target.\n",attacker.player.name);
        }
        return false;
    }

    if (isTeam1) team1stats.shotsOnTarget++; 
    else team2stats.shotsOnTarget++;

    double shotchance = Sigmoid(attacker.player.finishing, gk.player.goalPrevention);
    double randnum = dist(gen);

    if (randnum < shotchance){
        attacker.rating+=1; // Increase p1 rating for goal
        gk.rating-=0.3; // Decreate keeper rating for failed save
        if (isTeam1) team1stats.goals++;
        else team2stats.goals++;
        if (show){
            fmt::print(fg(fmt::color::green),"GOAL! ");
            if (isTeam1) fmt::print("{} scores for {}!\n",attacker.player.name,team1stats.team.name);
            else fmt::print("{} scores for {}!\n",attacker.player.name,team2stats.team.name);
        }
        return true;
    }
    else{
        attacker.rating+=0.2; // Slightly increase rating for shot on target
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

bool AttemptShot(Game& game, TeamGameStats& team1stats, TeamGameStats& team2stats, PlayerInGame& attacker, bool isTeam1, bool show = true){
    PlayerInGame gk = isTeam1 ? team2stats.players[0] : team1stats.players[0];

    if (isTeam1) team1stats.shots++;
    else team2stats.shots++;

    if (isTeam1) game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> [{}] {} attempts a shot.",team1stats.team.name, attacker.player.name)));
    else game.AddToLog(LogItem(ItemType::TextLine,fmt::format("> [{}] {} attempts a shot.",team2stats.team.name, attacker.player.name)));
    game.AddToLog(LogItem(ItemType::Text,"> "));

    std::default_random_engine gen = CreateGenerator();
    std::uniform_real_distribution<double> dist(0,1);

    double hitchance = GenerateOneNormal(40,25);

    if (attacker.player.finishing < hitchance){
        attacker.rating-=0.3; // Lower rating for missed shot
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::red), "MISS. ")));
        game.AddToLog(LogItem(ItemType::Text,fmt::format("{} missed the target.\n",attacker.player.name)));
        return false;
    }

    if (isTeam1) team1stats.shotsOnTarget++;
    else team2stats.shotsOnTarget++;

    double shotchance = Sigmoid(attacker.player.finishing, gk.player.goalPrevention);
    double randnum = dist(gen);

    if (randnum < shotchance){
        attacker.rating+=1; // Increase p1 rating for goal
        if (isTeam1) team2stats.players[0].rating-=0.5; // Decrease keeper rating for failed save
        else team1stats.players[0].rating-=0.5;
        if (isTeam1) team1stats.goals++;
        else team2stats.goals++;
        game.AddToLog(LogItem(ItemType::Text,fmt::format(fg(fmt::color::lime_green),"GOAL! ")));
        if (isTeam1) game.AddToLog(LogItem(ItemType::Text,fmt::format("{} scores for {}!\n",attacker.player.name,team1stats.team.name)));
        else game.AddToLog(LogItem(ItemType::Text,fmt::format("{} scores for {}!\n",attacker.player.name,team2stats.team.name)));
        // fmt::print(fg(fmt::color::green),"GOAL! ");
        // if (isTeam1) fmt::print("{} scores for {}!\n",p1.name,team1stats.team.name);
        // else fmt::print("{} scores for {}!\n",p1.name,team2stats.team.name);
        return true;
    }
    else{
        attacker.rating+=0.3; // Slightly increase rating for shot on target
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

int EndGameMenu(){
    int option = -1;
    std::string input;

    fmt::print("1. Display Goalscorers\n");
    fmt::print("2. Display Game Stats\n");
    fmt::print("3. Display Player Ratings\n");
    fmt::print("4. Simulate Game Again (Step by Step)\n");
    fmt::print("5. Simulate Game Again (Skip to Result)\n");
    fmt::print("0. Exit\n");
    fmt::print("---\n> ");

    std::cin >> input;

    if (!isStringInt(input)) option = -1; // If input is not a number set to default
    else if (std::stoi(input) > 9 || std::stoi(input) < 0) option = -1; // If input is number but out of range set to default
    else option = std::stoi(input); // Otherwise convert to integer

    return option;
}

void PrintStats(TeamGameStats team1, TeamGameStats team2){
    team1.CalculatePassAccuracy();
    team1.CalculateShotAccuracy();
    team2.CalculatePassAccuracy();
    team2.CalculateShotAccuracy();

    fmt::print("GAME STATS\n");

    size_t padValue = 5;
    if (team1.team.name.size() > padValue) padValue = team1.team.name.size();
    if (team2.team.name.size() > padValue) padValue = team2.team.name.size();

    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.team.name, team2.team.name, "", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.goals, team2.goals, "Goals", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",(std::to_string(team1.posession) + "%"), (std::to_string(team2.posession) + "%"), "Posession", padValue); // Add %
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.shots, team2.shots, "Shots", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.shotsOnTarget, team2.shotsOnTarget, "Shots On Target", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.passesAttempted, team2.passesAttempted, "Passes Attempted", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.passes, team2.passes, "Passes Completed", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.saves, team2.saves, "Saves", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.tackles, team2.tackles, "Tackles", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.interceptions, team2.interceptions, "Interceptions", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.fouls, team2.fouls, "Fouls", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.yellowCards, team2.yellowCards, "Yellow Cards", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",team1.redCards, team2.redCards, "Red Cards", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",(std::to_string(team1.shotAccuracy) + "%"), (std::to_string(team2.shotAccuracy) + "%"), "Shot Accuracy", padValue);
    fmt::print("{2:<17} {0:>{3}} - {1:<{3}}\n",(std::to_string(team1.passAccuracy) + "%"), (std::to_string(team2.passAccuracy) + "%"), "Pass Accuracy", padValue);
}

void PrintRatings(TeamGameStats team1, TeamGameStats team2){
    fmt::print("PLAYER RATINGS: \n");
    fmt::print("{}: \n",team1.team.name);

    fmt::print("--------[9]-------\n------------------\n[7]----[10]---[11]\n------------------\n----[6]-----[8]---\n------------------\n[2]--[4]--[5]--[3]\n--------[1]-------\n\n");
    /*foreach (var p in t1){
        Console.Write(p.Player.ToString() + ", "); 
        PrintRatingWithColour(p.Rating);
    }*/

    for (PlayerInGame p : team1.players){
        fmt::print("{}, ",p.player.ToString());
        PrintRatingWithColour(p.rating);
    }

    fmt::print("\n{}: \n",team2.team.name);
    
    fmt::print("\n--------[9]-------\n------------------\n[7]----[10]---[11]\n------------------\n----[6]-----[8]---\n------------------\n[2]--[4]--[5]--[3]\n--------[1]-------\n\n");
    /*foreach (var p in t2){
        Console.Write(p.Player.ToString() + ", "); 
        PrintRatingWithColour(p.Rating);
    }*/

    for (PlayerInGame p : team2.players){
        fmt::print("{}, ",p.player.ToString());
        PrintRatingWithColour(p.rating);
    }
}

void SimulateGameOld(Team team1, Team team2, bool show = true){
    fmt::print("Welcome to the game between {} and {}!\n\n",team1.name, team2.name);

    int team1_score = 0;
    int team2_score = 0;

    std::vector<Goal> goals;

    Player gk1 = team1.players[0];
    Player gk2 = team2.players[0];

    TeamGameStats team1stats = TeamGameStats(team1);
    TeamGameStats team2stats = TeamGameStats(team2);

    std::vector<PlayerInGame> playersInGame1;
    for (Player p : team1.players){
        playersInGame1.push_back(PlayerInGame(p)); // Double check this does what I expect
    }
    std::vector<PlayerInGame> playersInGame2;
    for (Player p : team2.players){
        playersInGame2.push_back(PlayerInGame(p)); // Double check this does what I expect
    }

    team1stats.players = playersInGame1;
    team2stats.players = playersInGame2;

    // std::default_random_engine gen = CreateGenerator();

    bool isTeam1 = true;
    
    if (show){
        fmt::print("Press ENTER to begin the game and to advance through the game.\n");
        system("pause");
        fmt::print("\n");
    }

    for (int i = 0; i <= 90; i+=3){ // Simulate in 5 minute increments
        std::string ord = "";
        if (i%10 == 1 && i!=11) ord = "st";
        else if (i%10 == 2 && i!=12) ord = "nd";
        else if (i%10 == 3 && i!=13) ord = "rd";
        else ord = "th";

        fmt::print("{}{} Minute\n",i,ord); // Print minute

        fmt::print("{} {} - {} {}\n===\n",team1.name, team1_score, team2_score, team2.name); // Print current score

        // Decide whether a pass, dribble or shot is attempted (0 = pass, 1 = dribble, 2 = shot)
        int choice = rand() % 3;

        // Pick random players to execute the actions
        PlayerInGame t1_p1 = playersInGame1[rand()%10+1]; // tX_pY = team X, player Y
        while (t1_p1.sentOff) t1_p1 = playersInGame1[rand()%10+1];
        PlayerInGame t1_p2 = playersInGame1[rand()%10+1];
        while (t1_p2.sentOff) t1_p2 = playersInGame1[rand()%10+1];
        PlayerInGame t2_p1 = playersInGame2[rand()%10+1];
        while (t2_p1.sentOff) t2_p1 = playersInGame2[rand()%10+1];
        PlayerInGame t2_p2 = playersInGame2[rand()%10+1];
        while (t2_p2.sentOff) t2_p2 = playersInGame2[rand()%10+1];

        if (isTeam1){ // TEAM 1
            team1stats.posession+=2;
            team2stats.posession-=2;

            Position pos = t1_p1.player.position;
            if (pos == Position::CB || pos == Position::LB || pos == Position::RB){
                if (choice == 2) choice = rand()%3; // if defender tries to shoot, attempt a reroll
            }
            else if (pos == Position::ST){
                if (choice != 2) choice = rand()%3; // if striker does anything but shoot, attempt a reroll
            }

            if (choice == 0){ // PASS
                fmt::print("> [{}] {} attempts to pass to {}.\n",team1.name, t1_p1.player.name, t1_p2.player.name);

                bool passed = AttemptPass(t1_p1, t1_p2, t2_p1); // Determine success=
                team1stats.passesAttempted++; // Increment attempted passes=

                if (passed){ // If pass was successful=
                    team1stats.posession+=3; // Alter possession numbers (refine these)
                    team2stats.posession-=3;
                    team1stats.passes++; // Incrememnt successful passes

                    // Alter player ratings
                    t1_p1.rating+=0.3; // Increase passer's rating
                    t1_p2.rating+=0.2; // Increase reciever's rating less
                    t2_p1.rating-=0.1; // Decrease failed intercepter's rating=

                    // If new player has high finishing and mentality, take a shot
                    if (t1_p2.player.finishing >= 50){
                        int a = rand() % 100;
                        if (t1_p2.player.mentality > a){
                            bool scored = AttemptShot(team1stats, team2stats, t1_p2, isTeam1, show); // Shoot

                            if (scored){
                                team1_score++;
                                ShowTeam1Scored(team1, team2, team1_score, team2_score);

                                goals.push_back(Goal(t1_p2.player.name, team1.name, i));
                            }
                        }
                    } // Otherwise attack fails 
                    // TODO: Chain passes? Dribbles? Save for better engine I think
                }
                else{ // Otherwise pass fails
                    team2stats.posession+=3;
                    team1stats.posession-=3;
                    team2stats.interceptions++;

                    // Alter player ratings
                    t1_p1.rating-=0.3; // Decrease passer's rating for failed pass
                    t1_p2.rating-=0.1; // Decrease reciever's rating less
                    t2_p1.rating+=0.3; // Increase intercepter's rating

                    // If intercepter has high finishing (or is a forward) and high mentality, take a shot
                    if (t2_p1.player.finishing >= 50 || t2_p1.player.position == Position::ST){
                        int a = rand() % 100;
                        if (t2_p1.player.mentality > a){
                            bool scored = AttemptShot(team1stats, team2stats, t2_p1, !isTeam1, show); // Shoot

                            if (scored){
                                team2_score++;
                                ShowTeam2Scored(team1, team2, team1_score, team2_score);

                                goals.push_back(Goal(t2_p1.player.name, team2.name, i));
                            }
                        }
                    }
                }
            }
            else if (choice == 1){ // DRIBBLE
                fmt::print("> [{}] {} attempts to dribble.\n",team1.name, t1_p1.player.name);
                
                bool dribbled = AttemptDribble(t1_p1, t2_p2, show);

                if (dribbled){
                    t1_p1.rating+=0.5; // Incremement attacker's rating for successful dribble
                    t2_p1.rating-=0.1; // Decrease defender's rating slightly for unsucessful tackle
                    team1stats.posession+=3; // Amend posession
                    team2stats.posession-=3;

                    // If player has high finishing and mentality, take a shot
                    if (t1_p1.player.finishing >= 50){
                        int a = rand() % 100;
                        if (t1_p1.player.mentality > a){
                            bool scored = AttemptShot(team1stats, team2stats, t1_p1, isTeam1, show); // Shoot

                            if (scored){
                                team1_score++;
                                ShowTeam1Scored(team1, team2, team1_score, team2_score);

                                goals.push_back(Goal(t1_p1.player.name, team1.name, i));
                            }
                        }
                    } // Otherwise attack fails 
                }
                else{
                    team2stats.tackles++;

                    // If tackling player has low mentality, have a chance to commit a foul
                    int b = rand() % 50;
                    if (t2_p1.player.mentality < b){
                        t2_p1.rating-=0.3; // Reduce rating for foul
                        team2stats.fouls++;

                        fmt::print("{} fouled {}.\n",t2_p1.player.name, t1_p1.player.name);

                        //Decide if a card should be shown
                        int yellowNum = GenerateOneNormal(30,10);
                        double yellowThreshold = Sigmoid(yellowNum, t2_p1.player.mentality,-0.05,0);
                        int redNum = GenerateOneNormal(5,2);
                        double redThreshold = Sigmoid(redNum, t2_p1.player.mentality, -0.063,0.5);
                        while (redThreshold > yellowThreshold){
                            redNum = GenerateOneNormal(5,2);
                            redThreshold = Sigmoid(redNum, t2_p1.player.mentality, -0.063,0.5);
                        }
                        double randNum = GenerateRandomDouble();

                        if (randNum < redThreshold){ // Player should be sent off
                            t2_p1.GiveCard(false);
                            t2_p1.rating-=2;
                            team2stats.redCards++;
                        }
                        else if (randNum < yellowThreshold){ // Player should get yellow card
                            t2_p1.GiveCard();
                            t2_p1.rating-=0.5;
                            team2stats.yellowCards++;
                        }

                        bool scored;
                        // If player is a striker, give a penalty (TODO: Refine this)
                        if (t1_p1.player.position == Position::ST){
                            scored = AttemptSetPiece(team1stats, team2stats, false, isTeam1, show);
                        }
                        // Otherwise if player plays forward, random chance of penalty or free kick based on attacker mentality
                        else if (t1_p1.player.position == Position::AM || t1_p1.player.position == Position::LW 
                            || t1_p1.player.position == Position::RW || t1_p1.player.position == Position::LM 
                            || t1_p1.player.position == Position::RM || t1_p1.player.position == Position::CM){
                            int c = rand()%100;
                            if (t1_p1.player.mentality > c){
                                scored = AttemptSetPiece(team1stats, team2stats, false, isTeam1, show); // Penalty
                            }
                            else{
                                scored = AttemptSetPiece(team1stats, team2stats, true, isTeam1, show); // FK
                            }
                        }
                        // Otherwise just a free kick (TODO: Make not always scorable from far back e.g. defenders?)
                        else{
                            scored = AttemptSetPiece(team1stats, team2stats, true, isTeam1, show); // FK
                        }

                        if (scored){
                            team1_score++;
                            ShowTeam1Scored(team1, team2, team1_score, team2_score);
                            goals.push_back(Goal(GetSetPieceTaker(team1stats).player.name, team1.name, i));
                        }
                    }
                    // Otherwise tackle is clean, and tackler can take a shot if high finishing and mentality
                    else{
                        t2_p1.rating+=0.5;
                        team2stats.posession+=3;
                        team1stats.posession-=3;

                        fmt::print("{} tackles {}.\n",t2_p1.player.name, t1_p1.player.name);

                        if (t2_p1.player.finishing >= 50){
                            int a = rand()%100;
                            if (t2_p1.player.mentality > a){
                                bool scored = AttemptShot(team1stats, team2stats, t2_p1, !isTeam1, show); // TOOD: Check this team is right
                                if (scored){
                                    team2_score++;
                                    ShowTeam2Scored(team1, team2, team1_score, team2_score);
                                    goals.push_back(Goal(t2_p1.player.name, team2.name, i));
                                }
                            }
                        }
                    }
                }
            }
            else{ // SHOOT
                team1stats.posession+=2;
                team2stats.posession-=2;

                bool scored = AttemptShot(team1stats, team2stats, t1_p1, isTeam1, show);
                if (scored){
                    team1_score++;
                    ShowTeam1Scored(team1, team2, team1_score, team2_score);
                    goals.push_back(Goal(t1_p1.player.name, team1.name, i));
                }
            }
        }
        else{ // TEAM 2
            team2stats.posession+=2;
            team1stats.posession-=2;

            Position pos = t2_p1.player.position;
            if (pos == Position::CB || pos == Position::LB || pos == Position::RB){
                if (choice == 2) choice = rand()%3; // if defender tries to shoot, attempt a reroll
            }
            else if (pos == Position::ST){
                if (choice != 2) choice = rand()%3; // if striker does anything but shoot, attempt a reroll
            }

            if (choice == 0){ // PASS
                fmt::print("> [{}] {} attempts to pass to {}.\n",team2.name, t2_p1.player.name, t2_p2.player.name);

                bool passed = AttemptPass(t2_p1, t2_p2, t1_p1); // Determine success=
                team2stats.passesAttempted++; // Increment attempted passes=

                if (passed){ // If pass was successful=
                    team2stats.posession+=3; // Alter possession numbers (refine these)
                    team1stats.posession+=3;
                    team2stats.passes++; // Incrememnt successful passes

                    // Alter player ratings
                    t2_p1.rating+=0.3; // Increase passer's rating
                    t2_p2.rating+=0.2; // Increase reciever's rating less
                    t1_p1.rating-=0.1; // Decrease failed intercepter's rating=

                    // If new player has high finishing and mentality, take a shot
                    if (t2_p2.player.finishing >= 50){
                        int a = rand() % 100;
                        if (t2_p2.player.mentality > a){
                            bool scored = AttemptShot(team1stats, team2stats, t2_p2, isTeam1, show); // Shoot

                            if (scored){
                                team2_score++;
                                ShowTeam2Scored(team1, team2, team1_score, team2_score);

                                goals.push_back(Goal(t2_p2.player.name, team2.name, i));
                            }
                        }
                    } // Otherwise attack fails 
                    // TODO: Chain passes? Dribbles? Save for better engine I think
                }
                else{ // Otherwise pass fails
                    team1stats.posession+=3;
                    team2stats.posession-=3;
                    team1stats.interceptions++;

                    // Alter player ratings
                    t2_p1.rating-=0.3; // Decrease passer's rating for failed pass
                    t2_p2.rating-=0.1; // Decrease reciever's rating less
                    t1_p1.rating+=0.3; // Increase intercepter's rating

                    // If intercepter has high finishing (or is a forward) and high mentality, take a shot
                    if (t1_p1.player.finishing >= 50 || t1_p1.player.position == Position::ST){
                        int a = rand() % 100;
                        if (t1_p1.player.mentality > a){
                            bool scored = AttemptShot(team1stats, team2stats, t1_p1, !isTeam1, show); // Shoot

                            if (scored){
                                team1_score++;
                                ShowTeam1Scored(team1, team2, team1_score, team2_score);

                                goals.push_back(Goal(t1_p1.player.name, team1.name, i));
                            }
                        }
                    }
                }
            }
            else if (choice == 1){ // DRIBBLE
                fmt::print("> [{}] {} attempts to dribble.\n",team2.name, t2_p1.player.name);
                
                bool dribbled = AttemptDribble(t2_p1, t1_p2, show);

                if (dribbled){
                    t2_p1.rating+=0.5; // Incremement attacker's rating for successful dribble
                    t1_p1.rating-=0.1; // Decrease defender's rating slightly for unsucessful tackle
                    team2stats.posession+=3; // Amend posession
                    team1stats.posession-=3;

                    // If player has high finishing and mentality, take a shot
                    if (t2_p1.player.finishing >= 50){
                        int a = rand() % 100;
                        if (t2_p1.player.mentality > a){
                            bool scored = AttemptShot(team1stats, team2stats, t2_p1, isTeam1, show); // Shoot

                            if (scored){
                                team2_score++;
                                ShowTeam2Scored(team1, team2, team1_score, team2_score);

                                goals.push_back(Goal(t2_p1.player.name, team2.name, i));
                            }
                        }
                    } // Otherwise attack fails 
                }
                else{
                    team1stats.tackles++;

                    // If tackling player has low mentality, have a chance to commit a foul
                    int b = rand() % 50;
                    if (t1_p1.player.mentality < b){
                        t1_p1.rating-=0.3; // Reduce rating for foul
                        team1stats.fouls++;

                        fmt::print("{} fouled {}.\n",t1_p1.player.name, t2_p1.player.name);

                        //Decide if a card should be shown
                        int yellowNum = GenerateOneNormal(30,10);
                        double yellowThreshold = Sigmoid(yellowNum, t1_p1.player.mentality,-0.05,0);
                        int redNum = GenerateOneNormal(5,2);
                        double redThreshold = Sigmoid(redNum, t1_p1.player.mentality, -0.063,0.5);
                        while (redThreshold > yellowThreshold){
                            redNum = GenerateOneNormal(5,2);
                            redThreshold = Sigmoid(redNum, t1_p1.player.mentality, -0.063,0.5);
                        }
                        double randNum = GenerateRandomDouble();

                        if (randNum < redThreshold){ // Player should be sent off
                            t1_p1.GiveCard(false);
                            t1_p1.rating-=2;
                            team1stats.redCards++;
                        }
                        else if (randNum < yellowThreshold){ // Player should get yellow card
                            t1_p1.GiveCard();
                            t1_p1.rating-=0.5;
                            team1stats.yellowCards++;
                        }

                        bool scored;
                        // If player is a striker, give a penalty (TODO: Refine this)
                        if (t2_p1.player.position == Position::ST){
                            scored = AttemptSetPiece(team1stats, team2stats, false, isTeam1, show);
                        }
                        // Otherwise if player plays forward, random chance of penalty or free kick based on attacker mentality
                        else if (t2_p1.player.position == Position::AM || t2_p1.player.position == Position::LW 
                            || t2_p1.player.position == Position::RW || t2_p1.player.position == Position::LM 
                            || t2_p1.player.position == Position::RM || t2_p1.player.position == Position::CM){
                            int c = rand()%100;
                            if (t2_p1.player.mentality > c){
                                scored = AttemptSetPiece(team1stats, team2stats, false, isTeam1, show); // Penalty
                            }
                            else{
                                scored = AttemptSetPiece(team1stats, team2stats, true, isTeam1, show); // FK
                            }
                        }
                        // Otherwise just a free kick (TODO: Make not always scorable from far back e.g. defenders?)
                        else{
                            scored = AttemptSetPiece(team1stats, team2stats, true, isTeam1, show); // FK
                        }

                        if (scored){
                            team2_score++;
                            ShowTeam2Scored(team1, team2, team1_score, team2_score);
                            goals.push_back(Goal(GetSetPieceTaker(team2stats).player.name, team2.name, i));
                        }
                    }
                    // Otherwise tackle is clean, and tackler can take a shot if high finishing and mentality
                    else{
                        t1_p1.rating+=0.5;
                        team1stats.posession+=3;
                        team2stats.posession-=3;

                        fmt::print("{} tackles {}.\n",t1_p1.player.name, t2_p1.player.name);

                        if (t1_p1.player.finishing >= 50){
                            int a = rand()%100;
                            if (t1_p1.player.mentality > a){
                                bool scored = AttemptShot(team1stats, team2stats, t1_p1, !isTeam1, show); // TOOD: Check this team is right
                                if (scored){
                                    team1_score++;
                                    ShowTeam1Scored(team1, team2, team1_score, team2_score);
                                    goals.push_back(Goal(t1_p1.player.name, team2.name, i));
                                }
                            }
                        }
                    }
                }
            }
            else{ // SHOOT
                team2stats.posession+=2;
                team1stats.posession-=2;

                bool scored = AttemptShot(team1stats, team2stats, t1_p1, isTeam1, show);
                if (scored){
                    team2_score++;
                    ShowTeam2Scored(team1, team2, team1_score, team2_score);
                    goals.push_back(Goal(t2_p1.player.name, team2.name, i));
                }
            }
        }

        fmt::print("\n");
        if (show) std::cin.get();
        isTeam1 = !isTeam1;
    }

    fmt::print(fg(fmt::color::red), "FULL TIME!\n");
    fmt::print("Final Score: {} {} - {} {}\n", team1.name, team1_score, team2_score, team2.name);

    fmt::print("\nAdditional Options: ");
    // int option = 0;
    // Do endgame menu
}

void SimulateGame(Team team1, Team team2, bool show = true){
    system("cls");
    fmt::print("Welcome to the game between {} and {}!\n\n",team1.name, team2.name);

    int team1_score = 0;
    int team2_score = 0;

    std::vector<Goal> goals;

    Player gk1 = team1.players[0];
    Player gk2 = team2.players[0];

    TeamGameStats team1stats = TeamGameStats(team1);
    TeamGameStats team2stats = TeamGameStats(team2);

    std::vector<PlayerInGame> playersInGame1;
    for (Player p : team1.players){
        playersInGame1.push_back(PlayerInGame(p)); // Double check this does what I expect
    }
    std::vector<PlayerInGame> playersInGame2;
    for (Player p : team2.players){
        playersInGame2.push_back(PlayerInGame(p)); // Double check this does what I expect
    }

    team1stats.players = playersInGame1;
    team2stats.players = playersInGame2;

    // std::default_random_engine gen = CreateGenerator();

    bool isTeam1 = true;
    
    if (show){
        fmt::print("Press ENTER to begin the game and to advance through the game.\n");
        fmt::print("Press any key to continue . . .");
        std::cin.get();
        fmt::print("\n");
    }

    for (int i = 0; i <= 90; i+=3){ // Simulate in 3 minute increments
        // Setup printing the time correctly
        std::string ord = "";
        if (i%10 == 1 && i!=11) ord = "st";
        else if (i%10 == 2 && i!=12) ord = "nd";
        else if (i%10 == 3 && i!=13) ord = "rd";
        else ord = "th";

        fmt::print("{}{} Minute\n",i,ord); // Print minute

        fmt::print("{} {} - {} {}\n===\n",team1.name, team1_score, team2_score, team2.name); // Print current score

        // Decide whether a pass, dribble or shot is attempted (0 = pass, 1 = dribble, 2 = shot)
        int choice = rand() % 3;

        // Pick 2 random players per team to execute the actions
        PlayerInGame *t1_p1;
        t1_p1 = &team1stats.players[rand()%10+1]; // tX_pY = team X, player Y
        while ((*t1_p1).sentOff) t1_p1 = &team1stats.players[rand()%10+1];

        PlayerInGame *t1_p2;
        t1_p2 = &team1stats.players[rand()%10+1];
        while ((*t1_p2).sentOff) t1_p2 = &team1stats.players[rand()%10+1];

        PlayerInGame *t2_p1;
        t2_p1 = &team2stats.players[rand()%10+1];
        while ((*t2_p1).sentOff) t2_p1 = &team2stats.players[rand()%10+1];

        PlayerInGame *t2_p2;
        t2_p2 = &team2stats.players[rand()%10+1];
        while ((*t2_p2).sentOff) t2_p2 = &team2stats.players[rand()%10+1];

        // Initialise pointers to variables and functions for attacking and defending teams
        // The attacking team is the team taking the action, and vice versa
        // The use of pointers like this means that I don't need to reuse much of the code, only changing variables
        // E.g. without this, I would have to do if (isTeam1) and rewrite the code for each scenario, only changing e.g. team1 to team2

        Team* attackingTeam;
        Team* defendingTeam;
        TeamGameStats *attackingTeamStats;
        TeamGameStats *defendingTeamStats;

        PlayerInGame *attacker1;
        PlayerInGame *attacker2;
        PlayerInGame *defender1;
        PlayerInGame *defender2;

        int *attacker_score;
        int *defender_score;

        // TODO: Verify function pointers do what I expect
        void (*ShowAttackingTeamScored)(Team team1, Team team2, int team1_score, int team2_score);
        void (*ShowDefendingTeamScored)(Team team1, Team team2, int team1_score, int team2_score);
        
        bool attacker; // For use in function calls requiring isTeam1 - true for Team 1, false for Team 2
        bool defender;

        // Assign pointers for if Team 1 is the attacking team, and vice versa for the else
        if (isTeam1){
            attackingTeam = &team1;
            defendingTeam = &team2;

            attackingTeamStats = &team1stats;
            defendingTeamStats = &team2stats;

            attacker1 = t1_p1;
            attacker2 = t1_p2;

            defender1 = t2_p1;
            defender2 = t2_p2;

            attacker_score = &team1_score;
            defender_score = &team2_score;

            ShowAttackingTeamScored = &ShowTeam1Scored;
            ShowDefendingTeamScored = &ShowTeam2Scored;

            attacker = true;
            defender = false;
        }
        else{
            attackingTeam = &team2;
            defendingTeam = &team1;

            attackingTeamStats = &team2stats;
            defendingTeamStats = &team1stats;

            attacker1 = t2_p1;
            attacker2 = t2_p2;

            defender1 = t1_p1;
            defender2 = t1_p2;

            attacker_score = &team2_score;
            defender_score = &team1_score;

            ShowAttackingTeamScored = &ShowTeam2Scored;
            ShowDefendingTeamScored = &ShowTeam1Scored;

            attacker = false;
            defender = true;
        }

        // ------------------------------------------------------
        // Now the setup is finished, we can begin the simulation

        // Base possession change for the beginning of an attack
        (*attackingTeamStats).posession+=2;
        (*defendingTeamStats).posession-=2;

        Position pos = (*attacker1).player.position;
        if (pos == Position::CB || pos == Position::LB || pos == Position::RB){
            if (choice == 2) choice = rand()%3; // If defender tries to shoot, attempt a reroll
        }
        else if (pos == Position::ST){
            if (choice != 2) choice = rand()%3; // If striker does anything but shoot, attempt a reroll
        }

        if (choice == 0){ // PASS
            fmt::print("> [{}] {} attempts to pass to {}.\n",(*attackingTeam).name, (*attacker1).player.name, (*attacker2).player.name);

            bool passed = AttemptPass(*attacker1, *attacker2, *defender1, show); // Determine success of pass
            (*attackingTeamStats).passesAttempted++; // Increment attempted passes counter

            if (passed){ // If pass was successful
                (*attackingTeamStats).posession+=3; // Alter posession numbers (TODO: Refine these)
                (*defendingTeamStats).posession-=3;
                (*attackingTeamStats).passes++; // Increment successful passes counter

                // Alter player ratings
                (*attacker1).rating+=0.3; // Increase passer's rating for successful pass
                (*attacker2).rating+=0.2; // Increase reciever's rating less
                (*defender1).rating-=0.1; // Slightly decrease intercepter's rating for failed interception

                // If reciever has high finishing and mentality, take a shot
                if ((*attacker2).player.finishing >= 50){
                    int a = rand() % 100;
                    if ((*attacker2).player.mentality > a){
                        bool scored = AttemptShot(team1stats, team2stats, *attacker2, attacker, show); // Shoot
                        if (scored){
                            (*attacker_score)++;
                            ShowAttackingTeamScored(team1, team2, team1_score, team2_score);
                            goals.push_back(Goal((*attacker2).player.name, (*attackingTeam).name, i));
                        }
                    }
                } // Otherwise attack fails
                // TODO: Chain passes? And dribbles? Maybe save for better engine
            }
            else{ // Otherwise pass is intercepted
                (*defendingTeamStats).posession+=3;
                (*attackingTeamStats).posession-=3;
                (*defendingTeamStats).interceptions++;

                (*attacker1).rating-=0.3; // Decrease passer's rating for failed pass
                (*attacker2).rating-=0.1; // Decrease reciever's ratiing less
                (*defender1).rating+=0.3; // Increase intercepter's rating for successful interception

                // If intercepter has high finishing (or is a forward) and mentality, take a shot
                if ((*defender1).player.finishing >= 50 || (*defender1).player.PositionType((*defender1).player.position) == Position::FW){
                    int a = rand() % 100;
                    if ((*defender1).player.mentality > a){
                        bool scored = AttemptShot(team1stats, team2stats, *defender1, defender, show);
                        if (scored){
                            (*defender_score)++;
                            ShowDefendingTeamScored(team1, team2, team1_score, team2_score);
                            goals.push_back(Goal((*defender1).player.name, (*defendingTeam).name, i));
                        }
                    }
                }
            }
        }
        else if (choice == 1){ // DRIBBLE
            fmt::print("> [{}] {} attempts to dribble.\n",(*attackingTeam).name, (*attacker1).player.name);

            bool dribbled = AttemptDribble(*attacker1, *defender1, show);

            if (dribbled){ // If dribble is successful
                (*attacker1).rating+=0.5; // Incremement attacker's rating for successful dribble
                (*defender1).rating-=0.1; // Decrease defender's rating slightly for unsucessful tackle
                (*attackingTeamStats).posession+=3; // Amend posession
                (*defendingTeamStats).posession-=3;

                // If attacking player has high finishing and mentality, take a shot
                if ((*attacker1).player.finishing >= 50){
                    int a = rand() % 100;
                    if ((*attacker1).player.mentality > a){
                        bool scored = AttemptShot(team1stats, team2stats, *attacker1, attacker, show);
                        if (scored){
                            (*attacker_score)++;
                            ShowAttackingTeamScored(team1, team2, team1_score, team2_score);
                            goals.push_back(Goal((*attacker1).player.name, (*attackingTeam).name, i));
                        }
                    }
                } // Otherwise attack fails
            }
            else{ // Otherwise player is tackled
                (*defendingTeamStats).tackles++;

                // If tackling player has low mentality, have a chance to commit a foul
                int b = rand() % 50;
                if ((*defender1).player.mentality < b){
                    (*defender1).rating-=0.3; // Reduce rating for foul
                    (*defendingTeamStats).fouls++;

                    fmt::print("{} fouled {}.\n", (*defender1).player.name, (*attacker1).player.name);

                    // Decide if a card should be shown
                    int yellowNum = GenerateOneNormal(30,10); // Generate random number for yellow card
                    double yellowThreshold = Sigmoid(yellowNum, (*defender1).player.mentality, -0.05, 0); // Determine mentality threshold for yellow card
                    int redNum = GenerateOneNormal(5,2);
                    double redThreshold = Sigmoid(redNum, (*defender1).player.mentality, -0.063, 0.5);
                    while (redThreshold > yellowThreshold){ // Reroll until red card threshold is lower than yellow card threshold
                        redNum = GenerateOneNormal(5,2);
                        redThreshold = Sigmoid(redNum, (*defender1).player.mentality, -0.063, 0.5);
                    }
                    double randNum = GenerateRandomDouble();

                    if (randNum < redThreshold){ // Player should be sent off
                        (*defender1).GiveCard(false);
                        (*defender1).rating-=2;
                        (*defendingTeamStats).redCards++;
                    }
                    else if (randNum < yellowThreshold){ // Player should get a yellow card
                        (*defender1).GiveCard(true);
                        (*defender1).rating-=0.5;
                        (*defendingTeamStats).yellowCards++;

                    } // Otherwise no card

                    bool scored;
                    // If attacking player is a striker, give a penalty (TODO: Refine this)
                    if ((*attacker1).player.position == Position::ST){
                        scored = AttemptSetPiece(team1stats, team2stats, false, attacker, show);
                    }
                    // Otherwise if player plays forward, winger or centrally midfield, give random chance of penalty or free kick based on attacker mentality
                    else if ((*attacker1).player.PositionType((*attacker1).player.position) == Position::FW 
                            || (*attacker1).player.position == Position::AM || (*attacker1).player.position == Position::LM 
                            || (*attacker1).player.position == Position::RM || (*attacker1).player.position == Position::CM){
                        int c = rand() % 100;
                        if ((*attacker1).player.mentality > c){
                            scored = AttemptSetPiece(team1stats, team2stats, false, attacker, show); // Penalty
                        }
                        else{
                            scored = AttemptSetPiece(team1stats, team2stats, true, attacker, show); // FK
                        }
                    }
                    // Otherwise just a free kick (TODO: Make these not always scoreable if they're from far back, e.g. defenders)
                    else{
                        scored = AttemptSetPiece(team1stats, team2stats, true, attacker, show); // FK
                    }

                    if (scored){
                        (*attacker_score)++;
                        ShowAttackingTeamScored(team1, team2, team1_score, team2_score);
                        goals.push_back(Goal(GetSetPieceTaker(team1stats).player.name, (*attackingTeam).name, i));
                    }
                }
                // Otherwise tackle is clean, and tackler can take shot if high finishing and mentality
                else{
                    (*defender1).rating+=0.5;
                    (*defendingTeamStats).posession+=3;
                    (*attackingTeamStats).posession-=3;

                    // fmt::print("> {} tackled {}.\n",(*defender1).player.name, (*attacker1).player.name);

                    if ((*defender1).player.finishing >= 50){
                        int a = rand()%100;
                        if ((*defender1).player.mentality > a){
                            bool scored = AttemptShot(team1stats, team2stats, *defender1, defender, show); // TOOD: Check this team is right
                            if (scored){
                                (*defender_score)++;
                                ShowDefendingTeamScored(team1, team2, team1_score, team2_score);
                                goals.push_back(Goal((*defender1).player.name, (*defendingTeam).name, i));
                            }
                        }
                    }
                }
            }
        }
        else{ // SHOT
            (*attackingTeamStats).posession+=2;
            (*defendingTeamStats).posession-=2;

            bool scored = AttemptShot(team1stats, team2stats, *attacker1, attacker, show);
            if (scored){
                (*attacker_score)++;
                ShowAttackingTeamScored(team1, team2, team1_score, team2_score);
                goals.push_back(Goal((*attacker1).player.name, (*attackingTeam).name, i));
            }
        }

        std::cin.clear();
        if (show) { fmt::print("\n"); 
        std::cin.get(); }
        isTeam1 = !isTeam1;
    }

    fmt::print(fg(fmt::color::red), "FULL TIME!\n");
    fmt::print("Final Score: {} {} - {} {}\n", team1.name, team1_score, team2_score, team2.name);

    fmt::print("\nAdditional Options: \n");
    int option = -1;
    // Do endgame menu

    while (option != 0 && option != 4 && option != 5){
        option = EndGameMenu();

        switch(option){
            case 1:
                fmt::print("\nGoalscorers:\n");
                for (Goal g : goals) g.GoalInfo();
                fmt::print("\n=========\n");
                system("pause");
                break;
            case 2:
                PrintStats(team1stats, team2stats);
                fmt::print("\n=========\n");
                system("pause");
                break;
            case 3:
                PrintRatings(team1stats, team2stats);
                fmt::print("\n=========\n");
                system("pause");
                break;
            case 4:
                SimulateGame(team1, team2, true);
                break;
            case 5:
                SimulateGame(team1, team2, false);
                break;
            case 0:
                break;
        }
    }
}