#include "UI.h"
#include "Restuarant.h"
#include <iostream>
using namespace std;

UI::UI() {}
UI::~UI() {}

bool UI::printStep(Restaurant* pRest, int timestep) {
    cout << "\n=============================================\n";
    cout << "Current Timestep: " << timestep << "\n";
    cout << "=============================================\n";

    // Call the restaurant's print function to show all lists
    pRest->printAll();

    cout << "\nPRESS ENTER TO CONTINUE, OR TYPE 'Q' TO END SIMULATION: ";
    string input;
    getline(cin, input); // Read whatever the user types

    if (input == "q" || input == "Q") {
        return false; // Tell the restaurant to stop
    }
    return true; // Tell the restaurant to keep going
}