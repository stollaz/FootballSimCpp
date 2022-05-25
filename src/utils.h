#include <string>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

// Function to check if a string contains only digits (e.g. it is an int)
bool isStringInt(std::string s) {
    for (int i = 0; i < s.length(); i++){
        if (!isdigit(s[i])) return false; // Currently this only checks if every char is a digit, could extend to include '.' and '-'
    }
    return true;
}

// Function to split a string based on a delimiter character
std::vector<std::string> Split(std::string s, char delimiter){
    std::stringstream st; // Initilaise stringstream
    st << s; // Add string to stream
    std::string segment; // Initialise segment and list
    std::vector<std::string> seglist;

    while (getline(st, segment, delimiter)) seglist.push_back(segment); // Iterate over segments and add to vector

    return seglist;
}

// Function to read in a file and return it as a vector of strings (vector of lines)
std::vector<std::string> ReadAllLines(fs::path p){
    std::vector<std::string> lines;
    std::ifstream f(p); // Open fine

    std::string line;
    while(getline(f, line)){ // Go through each line
        lines.push_back(line); // Add line to vector
    }

    return lines;
}