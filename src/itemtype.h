#ifndef ITEMTYPE
#define ITEMTYPE

// Enum to store type of action stored in playback
enum ItemType{
    Text,
    Colour,
    Read,
    TextLine
};

static const char *itemtype_str[] = {     
    "Text",
    "Colour",
    "Read",
    "TextLine"
};

#endif