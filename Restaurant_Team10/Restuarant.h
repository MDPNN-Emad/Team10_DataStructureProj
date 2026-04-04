#ifndef RESTAURANT_H
#define RESTAURANT_H

#include "Order.h"
#include "Cheff.h"
#include "Scooter.h"
#include "Table.h"
#include "LinkedQueue.h"
#include "PriQueue.h"

class UI;

class Restaurant {
private:
    UI* pUI;

    // --- PENDING LISTS ---
    LinkedQueue<Order*> PEND_ODG;
    LinkedQueue<Order*> PEND_ODN;
    LinkedQueue<Order*> PEND_OT;
    LinkedQueue<Order*> PEND_OVN;
    LinkedQueue<Order*> PEND_OVC; // Derived queue recommended in Phase 2 for cancellation
    priQueue<Order*> PEND_OVG;    // Priority Queue recommended

    // --- COOKING & READY LISTS ---
    priQueue<Order*> COOK_ORDS;   // Priority Queue recommended for cooking
    LinkedQueue<Order*> RDY_OD;
    LinkedQueue<Order*> RDY_OT;
    LinkedQueue<Order*> RDY_OV;

    // --- IN-SERVICE, FINISHED, CANCELLED ---
    priQueue<Order*> IN_SERV;     // Priority Queue recommended
    LinkedQueue<Order*> FIN_ORDS; // Queue or Stack accepted
    LinkedQueue<Order*> CANCELLED;

    // --- RESOURCES ---
    // Chefs
    LinkedQueue<Chef*> FREE_CN;
    LinkedQueue<Chef*> FREE_CS;

    // Scooters
    priQueue<Scooter*> FREE_SCOOTERS;
    LinkedQueue<Scooter*> BACK_SCOOTERS;
    LinkedQueue<Scooter*> MAINT_SCOOTERS;

    // Tables
    priQueue<Table*> FREE_TABLES;
    priQueue<Table*> BUSY_SHARABLE;
    priQueue<Table*> BUSY_NO_SHARE;

    // Helper functions for phase 1.2
    int getQueueCount(LinkedQueue<Order*>& q);
    int getPriQueueCount(priQueue<Order*>& q);

public:
    Restaurant();
    ~Restaurant();
    void randomSimulation();
    void printAll();
};

#endif