#pragma once
#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <ostream>
using namespace std;

class Table {
private:
    int id;
    int capacity;               // Total number of seats (from input)
    int occupiedSeats;          // Currently used seats (for table sharing)
    int occupiedUntil;          // Timestep when table becomes free (0 = free)

    int orderIDs[10];  // assuming Max 10 orders per table (used in phase 2)
    int orderCount;    // How many currently stored

public:
    // Constructor
    Table(int id, int capacity)
        : id(id), capacity(capacity), occupiedSeats(0), occupiedUntil(0), orderCount(0) {
    }

    // Getters
    int getID() const { return id; }
    int getCapacity() const { return capacity; }
    int getOccupiedSeats() const { return occupiedSeats; }
    int getFreeSeats() const { return capacity - occupiedSeats; }
    int getOccupiedUntil() const { return occupiedUntil; }
    int getOrderCount() const { return orderCount; }

    // Get order ID at specific index
    int getOrderID(int index) const {
        if (index >= 0 && index < orderCount) {
            return orderIDs[index];
        }
        return -1;  // Invalid index
    }

    // Check if table is available at given time (has free seats and is not occupied beyond current time)
    bool isAvailable(int currentTime) const {
        return (occupiedUntil <= currentTime) && (getFreeSeats() > 0);
    }

    // Check if table can fit a given number of seats
    bool canFit(int seats) const {
        return getFreeSeats() >= seats;
    }

    // Assign an order to this table (dine-in, sharable case)
    // duration calculation
    bool assignOrder(int orderID, int seats, int duration, int currentTime) {
        // Check if table is available 
        if (occupiedUntil > currentTime) {
            return false; 
        }

        // 2. Check if enough free seats
        if (getFreeSeats() < seats) {
            return false;  
        }

        // 3. Check if we can store another order ID (I assumed max of 10)
        if (orderCount >= 10) {
            return false;   
        }

        // Now assign
        orderIDs[orderCount] = orderID;
        orderCount++;
        occupiedSeats += seats;

        // Update occupiedUntil (table becomes free after the order duration)
        int newFreeTime = currentTime + duration;
        if (newFreeTime > occupiedUntil) {
            occupiedUntil = newFreeTime;
        }

        return true;
    }

    // Free the table completely (when all orders finish)
    void freeTable(int currentTime) {
        if (occupiedUntil <= currentTime) {
            
            occupiedSeats = 0;
            occupiedUntil = 0;
            orderCount = 0; //ik there will be memory waste ( I'll make linked list in phase 2)
        }
    }

    // Remove a specific order (cancellation) for shared tables
    void removeOrder(int orderID, int seats) {
        // Finding the order 
        for (int i = 0; i < orderCount; i++) {
            if (orderIDs[i] == orderID) {
                // Shifting
                for (int j = i; j < orderCount - 1; j++) {
                    orderIDs[j] = orderIDs[j + 1];
                }
                orderCount--;
              
                // Update occupied seats
                occupiedSeats -= seats;


                // If no orders left, table becomes completely free
                if (orderCount == 0) {
                    occupiedSeats = 0;
                    occupiedUntil = 0;
                }
                return;  
            }
        }
        cout << "order not found!" << endl;
    }

    // Friend operator<< to access members
    friend ostream& operator<<(ostream& os, const Table* table) {
        if (table) {
            os << "[T" << table->id
                << ", " << table->capacity
                << ", " << table->getFreeSeats() << "]";
        }
        else {
            os << "[null]";
        }
        return os;
    }
};

#endif // TABLE_H
