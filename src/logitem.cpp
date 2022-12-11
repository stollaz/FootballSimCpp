#include "logitem.h"
#include <algorithm>

LogItem::LogItem(ItemType type, std::string value){
    m_itemType = type;
    m_itemValue = value;
}

LogItem::LogItem(std::string type, std::string value){
    // Convert string to lower case
    std::for_each(type.begin(), type.end(), [](char & c) {
        c = ::toupper(c);
    });

    if (type == "text" || type == "string") m_itemType = ItemType::Text;
    else if (type == "textline" || type == "stringline") m_itemType = ItemType::Text;
    else if (type == "color" || type == "colour") m_itemType = ItemType::Colour;
    else if (type == "read" || type == "input" || type == "enter") m_itemType = ItemType::Read;
    else m_itemType = ItemType::Text;
    m_itemValue = value;
}