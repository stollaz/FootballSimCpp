#include <string>

#ifndef GOAL
#define GOAL

class Goal{
    private:
        std::string name;
        std::string team;
        int minute;
    public:
        Goal(std::string _name, std::string _team, int _minute);
        void GoalInfo();
};

#endif