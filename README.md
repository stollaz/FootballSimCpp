# FootballSimCpp

My football simulator from https://github.com/stollaz/FootballSimv2, ported to C++ (as of latest version a.2.2021.12.23.0)

---
## Why?

Many game development jobs in the industry, including Sports Interactive (makers of Football Manager), require C++ experience. Porting my own simulator code from C# to C++ gives me experience in programming in C++ to prepare for the industry.

---
## Introduction
This basic Football Simulator aims to be a simple yet somewhat realistic simulator of association football matches. There aims to be two major aspects to the program:

1. Simulation of games involving randomly generated players, teams and stats
2. Simulation of games involving real life players, teams and stats

The idea started based on the idea of aspect 1. The early program would (and still can) generate a random team name using a location prefix 
(town name, selected from a file containing every town in the UK), and a standard football team name suffix (selected from a similar file 
of such suffixes, e.g. City, Town, F.C., etc.). 

There are two more aspects which each of the prior can be split into:

1. Simulation of a single game, stepping through the game in fixed-time intervals (currently and be default 3 minutes), wherein during each interval, a random attacking 
action is performed by a player of one team (A), and a player on the other team (B) attempt to defend against. These actions may be attempting a shot on goal, attempting 
to pass the ball, or attempting to dribble the ball. The player's generated stats are used in conjunction with random sampling on a sigmoid function, where identical statistical
values imply a 50% chance of either player winning the situation.
2. Simulation of an entire season, simulating quickly each game in the way outlined above, before displaying a final league table.

## Real Data
Data about real players is obtained by webscraping fbref.com, a football statistic site. These statistics are then interpreted into a number of attributes assigned to a player, 
and these attributes dictate how a player performs. [...]

---
## Changelog a.3.2022.05.24.0

- Initial C++ port
- Implemented Player class (player.h and player.cpp)
- Implemented Position enum (position.h)
- Implemented ItemType enum (itemtype.h)
- Imported "fmt" library for string formatting
- Imported assets used in FootballSimv2 project
  - Plan is to update the stats for the 2021/22 season
    - As of now, been softblocked due to webscraping
  - Also plan to fix the random name generation, e.g. to separate male and female names so they don't mix in one simulation

#### Changelog a.3.2022.05.24.1
- Fixed name of ItemType enum