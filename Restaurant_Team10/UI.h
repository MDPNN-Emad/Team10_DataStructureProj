#ifndef UI_H
#define UI_H

class Restaurant; // Forward declaration

class UI {
public:
    UI();
    ~UI();
    bool printStep(Restaurant* pRest, int timestep);
};
#endif