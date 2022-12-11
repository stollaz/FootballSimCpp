#ifndef POSITION
#define POSITION

// Enum to store position of player
enum Position{
    GK,
    LB,
    CB,
    RB,
    DM,
    CM,
    AM,
    LM,
    RM,
    RW,
    LW,
    ST,
    DF,
    MF,
    FW
};

// Array of strings to store string representation of enum
static const char *position_str[] = {     
    "GK",
    "LB",
    "CB",
    "RB",
    "DM",
    "CM",
    "AM",
    "LM",
    "RM",
    "RW",
    "LW",
    "ST",
    "DF",
    "MF",
    "FW"
};

#endif