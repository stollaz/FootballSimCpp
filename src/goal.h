#include <string>

#ifndef GOAL
#define GOAL

class Goal{
    public:
        Goal(std::string _name, std::string _team, int _minute);
        void PrintGoalInfo();
    private:
        std::string m_name;
        std::string m_team;
        int m_minute;
};

#endif