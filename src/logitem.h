#include <string>
#include "itemtype.h"

#ifndef LOGITEM
#define LOGITEM

class LogItem{
    private:
    public:
        ItemType itemType;
        std::string itemValue;

        LogItem(ItemType type, std::string value);
        LogItem(std::string type, std::string value);
};

#endif