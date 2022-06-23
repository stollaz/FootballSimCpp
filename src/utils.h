#include <string>
#include <vector>
#include <chrono>

#include <filesystem>
namespace fs = std::filesystem;

// Function to check if a string contains only digits (e.g. it is an int)
bool isStringInt(std::string s) {
    for (size_t i = 0; i < s.length(); i++){
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
    
    f.close();

    return lines;
}

// Function to count the lines in a file
int CountLines(fs::path p){
    std::ifstream f(p);
    
    int count = 0;

    std::string line;
    while(getline(f,line)) count++;

    f.close();

    return count;
}

// Function to create a probability based on the similarity of two stats, along with tightness and offset parameters
double Sigmoid(int stat1, int stat2, double tightness =-0.05, double offset = 0.5){
    int x = stat1 - stat2;
    double b = tightness;
    return 1/(1+exp(offset+b*x));
}

// Create a random generator for use in generating random numbers
// One generator should be used for many random numbers to avoid repeats, so this should only be created
// for each batch of random numbers, not for each individual number
std::default_random_engine CreateGenerator() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count()*std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    return generator;
}

// Create a normal distribution with the specificed mean and standard deviation
std::normal_distribution<double> CreateNormalGenerator(int mean, float std) {
    std::normal_distribution<double> dist(mean,std);

    return dist;
}

// This doesn't work
int GenerateNormal(std::default_random_engine generator, std::normal_distribution<double> distribution){
    return (int)distribution(generator);
}

// Only use this version when generating a single normal distribution number
// If multiple are generated in quick succession, it will output duplicates
int GenerateOneNormal(int mean, float std){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count()*std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> dist(mean,std);

    return (int)dist(generator);
}

double GenerateRandomDouble(double lower = 0.0, double higher = 1.0){
    std::uniform_real_distribution<double> unif(lower,higher);
    std::default_random_engine re;
    return unif(re);

}

fmt::color GetColour(int x){
    if (x >= 90) return fmt::color::turquoise;
    else if (x >= 75) return fmt::color::lime_green;
    else if (x >= 50) return fmt::color::yellow;
    else if (x >= 30) return fmt::color::red;
    else return fmt::color::dark_red;
}

void PrintWithColour(int x){
    fmt::color c = GetColour(x);
    fmt::print(fg(c),"{}\n", x);
}

void PrintPositionWithColour(int x){
    fmt::color c = GetColour(x);
    fmt::print("[");
    fmt::print(fg(c),"{}", x);
    fmt::print("]\n");
}

void PrintRatingWithColour(double rating, bool newline = true){
    fmt::color c;
    if (rating <= 5.0) c = fmt::color::indian_red;
    else if (rating < 6.0) c = fmt::color::red;
    else if (rating < 6.5) c = fmt::color::orange;
    else if (rating < 7.0) c = fmt::color::yellow;
    else if (rating < 8.0) c = fmt::color::forest_green;
    else if (rating < 9.0) c = fmt::color::lime_green; 
    else if (rating < 10.0) c = fmt::color::turquoise;
    else c = fmt::color::dark_turquoise;

    fmt::print(fg(c),"{0:.1f}",rating);
    if (newline) fmt::print("\n");
}

void PrintRatingWithColour(PlayerInGame p, bool newline = true){
    double rating = p.rating;
    fmt::color c;
    if (rating <= 5.0) c = fmt::color::indian_red;
    else if (rating < 6.0) c = fmt::color::red;
    else if (rating < 6.5) c = fmt::color::orange;
    else if (rating < 7.0) c = fmt::color::yellow;
    else if (rating < 8.0) c = fmt::color::forest_green;
    else if (rating < 9.0) c = fmt::color::lime_green; 
    else if (rating < 10.0) c = fmt::color::turquoise;
    else c = fmt::color::dark_turquoise;

    fmt::print("{}: ",p.player.name);
    fmt::print(fg(c),"{0:.1f}",rating);
    if (newline) fmt::print("\n");
}

// END