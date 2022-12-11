#include "goal.h"
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"

Goal::Goal(std::string _name, std::string _team, int _minute){
    m_name = _name;
    m_team = _team;
    m_minute = _minute;
}

void Goal::PrintGoalInfo(){
    fmt::print("{}' - {} [{}]\n", m_minute, m_name, m_team);
}