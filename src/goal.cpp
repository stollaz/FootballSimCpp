#include "goal.h"
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"

Goal::Goal(std::string _name, std::string _team, int _minute){
    Goal::name = _name;
    Goal::team = _team;
    Goal::minute = _minute;
}

void Goal::GoalInfo(){
    fmt::print("{}' - {} [{}]\n", minute, name, team);
}