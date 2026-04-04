#include "Restuarant.h"
#include "UI.h"
#include "random_generator.h" // <--- Included your random generator
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

Restaurant::Restaurant() {
    pUI = new UI();
    srand(time(0));
}

Restaurant::~Restaurant() {
    delete pUI;

    Order* tempOrd; int p;
    while (PEND_ODG.dequeue(tempOrd)) delete tempOrd;
    while (PEND_ODN.dequeue(tempOrd)) delete tempOrd;
    while (PEND_OT.dequeue(tempOrd)) delete tempOrd;
    while (PEND_OVN.dequeue(tempOrd)) delete tempOrd;
    while (PEND_OVC.dequeue(tempOrd)) delete tempOrd;
    while (PEND_OVG.dequeue(tempOrd, p)) delete tempOrd;

    while (COOK_ORDS.dequeue(tempOrd, p)) delete tempOrd;
    while (RDY_OD.dequeue(tempOrd)) delete tempOrd;
    while (RDY_OT.dequeue(tempOrd)) delete tempOrd;
    while (RDY_OV.dequeue(tempOrd)) delete tempOrd;
    while (IN_SERV.dequeue(tempOrd, p)) delete tempOrd;
    while (FIN_ORDS.dequeue(tempOrd)) delete tempOrd;
    while (CANCELLED.dequeue(tempOrd)) delete tempOrd;

    Chef* tempChef;
    while (FREE_CN.dequeue(tempChef)) delete tempChef;
    while (FREE_CS.dequeue(tempChef)) delete tempChef;

    Scooter* tempScooter;
    while (FREE_SCOOTERS.dequeue(tempScooter, p)) delete tempScooter;

    Table* tempTable;
    while (FREE_TABLES.dequeue(tempTable, p)) delete tempTable;
}

int Restaurant::getQueueCount(LinkedQueue<Order*>& q) {
    int count = 0; Order* temp; LinkedQueue<Order*> tempQueue;
    while (q.dequeue(temp)) { count++; tempQueue.enqueue(temp); }
    while (tempQueue.dequeue(temp)) { q.enqueue(temp); }
    return count;
}

int Restaurant::getPriQueueCount(priQueue<Order*>& q) {
    int count = 0; Order* temp; int p; priQueue<Order*> tempQueue;
    while (q.dequeue(temp, p)) { count++; tempQueue.enqueue(temp, p); }
    while (tempQueue.dequeue(temp, p)) { q.enqueue(temp, p); }
    return count;
}

void Restaurant::printAll() {
    cout << "--- PENDING ORDERS ---\n";
    cout << "ODG: "; PEND_ODG.printQueue();
    cout << "ODN: "; PEND_ODN.printQueue();
    cout << "OT: "; PEND_OT.printQueue();
    cout << "OVN: "; PEND_OVN.printQueue();
    cout << "OVC: "; PEND_OVC.printQueue();
    cout << "OVG: "; PEND_OVG.printQueue();

    cout << "\n--- RESOURCES ---\n";
    cout << "Free Tables: "; FREE_TABLES.printQueue();
    cout << "Free Scooters: "; FREE_SCOOTERS.printQueue();
    cout << "Free Chefs (CN): "; FREE_CN.printQueue();

    cout << "\n--- PROCESSING ORDERS ---\n";
    cout << "Cooking: "; COOK_ORDS.printQueue();
    cout << "Ready (OT): "; RDY_OT.printQueue();
    cout << "In Service: "; IN_SERV.printQueue();
    cout << "Finished: "; FIN_ORDS.printQueue();
    cout << "Cancelled: "; CANCELLED.printQueue();
}

void Restaurant::randomSimulation() {
    // Mode selection REMOVED as requested

    // 1. Initialize Tables
    for (int i = 1; i <= 5; i++) FREE_TABLES.enqueue(new Table(i, rand() % 5 + 2), 10);

    // 2. Initialize Scooters
    for (int i = 1; i <= 5; i++) FREE_SCOOTERS.enqueue(new Scooter(i, 50, 3, 5), 10);

    // 3. Initialize Chefs
    for (int i = 1; i <= 5; i++) FREE_CN.enqueue(new Chef(i, "CN", 3));
    for (int i = 6; i <= 8; i++) FREE_CS.enqueue(new Chef(i, "CS", 5));

    // 4. Initialize 500 random pending orders using YOUR RandomGenerator
    RandomGenerator rng;
    for (int i = 1; i <= 500; i++) {
        Order* newOrder = rng.generate_random_order(i, 1);
        string t = newOrder->get_type();

        if (t == "ODG") PEND_ODG.enqueue(newOrder);
        else if (t == "ODN") PEND_ODN.enqueue(newOrder);
        else if (t == "OT") PEND_OT.enqueue(newOrder);
        else if (t == "OVC") PEND_OVC.enqueue(newOrder);
        else if (t == "OVN") PEND_OVN.enqueue(newOrder);
        else if (t == "OVG") PEND_OVG.enqueue(newOrder, rand() % 10); // Priority Queue
    }

    int timestep = 1;

    // Loop until all 500 orders are finished or cancelled
    while ((getQueueCount(FIN_ORDS) + getQueueCount(CANCELLED)) < 500) {
        Order* pOrd = nullptr; int pri;

        // Move FCFS Pending to Cooking
        for (int i = 0; i < 5; i++) {
            if (PEND_ODG.dequeue(pOrd)) COOK_ORDS.enqueue(pOrd, rand() % 10);
            if (PEND_OT.dequeue(pOrd)) COOK_ORDS.enqueue(pOrd, rand() % 10);
            if (PEND_OVG.dequeue(pOrd, pri)) COOK_ORDS.enqueue(pOrd, rand() % 10);
        }

        // Move Cooking to Ready
        if (rand() % 100 < 75) {
            for (int i = 0; i < 10; i++) {
                if (COOK_ORDS.dequeue(pOrd, pri)) {
                    if (pOrd->get_type() == "OT") RDY_OT.enqueue(pOrd);
                    else RDY_OD.enqueue(pOrd);
                }
            }
        }

        // Move Ready to InService or Finished (if OT)
        for (int i = 0; i < 10; i++) {
            if (RDY_OT.dequeue(pOrd)) FIN_ORDS.enqueue(pOrd);
            if (RDY_OD.dequeue(pOrd)) IN_SERV.enqueue(pOrd, rand() % 10);
        }

        // Finish In-Service Orders
        if (rand() % 100 < 25) {
            if (IN_SERV.dequeue(pOrd, pri)) FIN_ORDS.enqueue(pOrd);
        }

        // ---> NEW EARLY EXIT LOGIC <---
        bool continueSim = pUI->printStep(this, timestep);
        if (!continueSim) {
            cout << "\nSimulation manually terminated by the user." << endl;
            break; // This kicks us out of the while loop immediately!
        }

        timestep++;
    }

    cout << "\nPhase 1.2 Simulation Finished at timestep " << timestep << "!" << endl;
}