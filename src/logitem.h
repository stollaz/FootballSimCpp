#include <string>
#include "itemtype.h"

#ifndef LOGITEM
#define LOGITEM

class LogItem{
    public:
        ItemType m_itemType;
        std::string m_itemValue;

        LogItem(ItemType type, std::string value);
        LogItem(std::string type, std::string value);
    
    private:
};

#endif