#include "logitem.h"
#include <algorithm>

LogItem::LogItem(ItemType type, std::string value){
    itemType = type;
    itemValue = value;
}

LogItem::LogItem(std::string type, std::string value){
    // Convert string to lower case
    std::for_each(type.begin(), type.end(), [](char & c) {
        c = ::toupper(c);
    });

    if (type == "text" || type == "string") itemType = ItemType::Text;
    else if (type == "textline" || type == "stringline") itemType = ItemType::Text;
    else if (type == "color" || type == "colour") itemType = ItemType::Colour;
    else if (type == "read" || type == "input" || type == "enter") itemType = ItemType::Read;
    else itemType = ItemType::Text;
    itemValue = value;
}