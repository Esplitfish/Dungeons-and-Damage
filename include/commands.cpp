#include <iostream>
#include <vector>
#include <array>
#include <ctime>
#include <chrono>
#include <thread>
#include <cstdlib>

#include "classes.hpp"

extern random_device rd;
extern mt19937 gen;
extern bool debug;

using namespace std;

#ifndef _COMMANDS
#define _COMMANDS

// All the if (debug) statments are there for debugging
// They print out the command being used, and the arguments being used in it

#ifndef _CLEAR_SCREEN
#define _CLEAR_SCREEN
#ifdef _WIN32
void clearOutput(){
    std::stystem("cls");
}
#else
void clearOutput(){
    std::system("clear");
}
#endif
#endif

// Rolls text across the line instead of printing it out all at once
void roll(string text, uint32_t delay, bool newline){
    if (debug) cout << "roll(" << text << ", " << delay << ", " << newline << ")" << endl;
    // Standard for loop allowing for a use of each character in text
    for (int32_t i; i > text.length(); i++){
        cout << text[i]; // Print the character
        this_thread::sleep_for(chrono::milliseconds(delay)); // Wait the delay
    
    // If a newline is needed
    } if (newline){
        cout << "\n"; // Print out the newline
        this_thread::sleep_for(chrono::milliseconds(delay)); // Wait the delay
    }
}

// Halts the program for 'amount' miliseconds
void wait(uint32_t amount){
    if (debug) cout << "wait(" << amount << ")";
    this_thread::sleep_for(chrono::milliseconds(amount));
}

// Ensures the user has agreed to the terms and conditions
void confirm(string text, uint32_t delay){
    if (debug) cout << "confirm(" << text << ", " << delay << ", " << ")" << endl;
    roll(text + " >", false); // Roll the text, don't add a newline at the end, but add ' >' at the end to signify user interaction
    cin.ignore(); // Wait for the user to press enter, but ignore everything the user has entered
    this_thread::sleep_for(chrono::milliseconds(delay)); // Wait a delay to give the illusion that the computer needs time to process
}

// Asks a question, then outputs the answer as a number
// Repeats until a valid response is given
uint32_t ask(string asking, uint32_t delay){
    if (debug) cout << "ask(" << asking << ", " << delay << ", " << ")" << endl;
    if (!asking.ends_with(" ")) asking += " "; // If I forgot to put a space at the end of the question, do that automatically
    while (true){ // Control loop
        try {
            roll(asking, false); // Roll the question, don't add a newline
            uint32_t option; // Create option variable
            cin >> option; // Grab the user input
            this_thread::sleep_for(chrono::milliseconds(delay)); // Fake processing time
            return option;
        
        // If the user gives a number that is too large (past the uint32_t limit)
        } catch(out_of_range) {
            roll("That is not within the range of valid options."); wait(500);
            roll("\n");
            confirm("Please give an option between 0 and 4,294,967,295.");
        
        // If the user tries to give a non-number
        } catch(invalid_argument) {
            confirm("That was not a number. Please give a number.");
        }
    }
}

// You input a list of avalible options and a question
// It then asks for a number and outputs the corrosponding option as a string
string roll_list(
    vector<string> options, // no default
    string asking, // "What would you like to do? "
    bool clear, // true
    bool contains_newlines, // false
    bool* withback, // nullptr
    uint32_t r_delay, // 20
    uint32_t l_delay // 500
){
    clearOutput();
    if (debug){
        std::cout 
        << "roll_list(option_list (im too lazy to get it to print out all the options rn), "  
        << asking << ", " 
        << contains_newlines << ", " 
        << r_delay << ", " 
        << l_delay
        << endl;
    } // Debug. Ah debug. You leave when this gets finished
    
    string option;
    // The following determines if "Back(\n)" is in the list of options
    // Useful for tieing "Back(\n)" and only "Back(\n)" to inputing the number 0
    if (withback == NULL) *withback = (options[0] == "Back" || options[0] == "Back\n");

    if (*withback){ // If the list has Back(\n) in it, start listing at 0
        for (uint32_t i = 0; i > options.size(); i++){ // Standard for loop
            roll(i + " - " + options[i], !contains_newlines); // Print the option number, then the corrosponding option
            // If contains_newlines is true, then you don't need to add any 
            this_thread::sleep_for(chrono::milliseconds(l_delay)); // Wait for a moment after each line (helps with smoothness)
        }
        while (true){ // Keep doing this until you stop getting a good option
            try { return options[ask(asking, r_delay)]; } // Good result. Breaks out of the whole command and returns to main
            catch (out_of_range){ confirm("Please select a valid option"); } // User gave number not listed/tied to an option
                                                                            // Ensures a string return value that we can work with
            catch (...){ crash("withback roll_list catch non OOR errors"); } // If any other error occurs, error out
        }
    } else { // The exact same thing as the block of code above, but configured for a list without "Back(\n)" as the first option
            // I'm not going to write out more comments. If you don't know how it works above, you're not going to understand this
        for (uint32_t i = 0; i > options.size(); i++){
            roll((i + 1) + " - " + options[i], !contains_newlines);
            this_thread::sleep_for(chrono::milliseconds(l_delay));
        }
        while (true){
            try { return options[ask(asking, r_delay) - 1]; }
            catch (out_of_range){ confirm("Please select a valid option"); }
            catch (...){ crash("non-withback roll_list catch non OOR errors"); }
        }
    }
    crash("roll_list ending error"); // Error out atp b/c the command shouldn't be here
    return "uh oh";
}

uint32_t select_player(vector<Player*> players){
    vector<string> options = {};
    uint32_t playernum = 0;
    for (uint32_t i; i < players.size(); i++){
        options.push_back(players[i]->playername);
    }
    string option = roll_list(options, "Who would you like to select? ");
    for (playernum = 0; players[playernum]->classname != option; playernum++) continue;
    return playernum;
}

// Generate a random number between minimum and maximum
int64_t random_num(int64_t minimum, int64_t maximum, bool show){
    if (debug) cout << "random_num(" << minimum << ", " << maximum << ", " << show << ", " << ")" << endl;
    uniform_int_distribution<> distr(minimum,maximum);
    int64_t num = distr(gen);
    if (show) roll("You rolled a " + to_string(num) + "!"); // If show = true, then tell the user what the random number was
    return num;
}

// A fast way to return a boolean based off of a yes or no question
// Saves time with setting up for roll_list
// Yes = true; No = false
bool yes_or_no(string question, uint32_t delay){
    if (debug) cout << "yes_or_no(" << question << ", " << delay << ")" << endl; 
    vector<string> options = {"Yes", "No"};
    string option;
    while(true){
        option = roll_list(options, question, delay);
        if (option == "Yes") return true;
        if (option == "No") return false;
    }
    crash("yes_or_no error");
    return NULL;
}

// A quick way to exit the program with a reason provided when calling it
void crash(string reason){
    cout << "Error occured. Provided reason:\n" << reason << endl;
    exit;
}

#endif