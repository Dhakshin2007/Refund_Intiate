#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

struct Partner {
    int id;
    double distToConsumer;
};

struct Warehouse {
    char id;
    double distFromConsumer;
    double trafficPercent;
};

// Comparator to sort partners by distance
bool comparePartners(const Partner &a, const Partner &b) {
    return a.distToConsumer < b.distToConsumer;
}

int main() {
    int numPartners, numWarehouses;
    
    // --- 1. Input Partners ---
    cout << "Enter number of Delivery Partners: ";
    cin >> numPartners;
    
    vector<Partner> partners(numPartners);
    for(int i = 0; i < numPartners; i++) {
        partners[i].id = i + 1;
        cout << "Enter Distance for Partner " << i + 1 << " to Consumer: ";
        cin >> partners[i].distToConsumer;
    }

    // --- 2. Input Warehouses ---
    cout << "\nEnter number of Warehouses: ";
    cin >> numWarehouses;
    
    vector<Warehouse> warehouses(numWarehouses);
    for(int i = 0; i < numWarehouses; i++) {
        warehouses[i].id = 'A' + i; // Assigns A, B, C...
        cout << "Enter Distance for Warehouse " << warehouses[i].id << " to Consumer: ";
        cin >> warehouses[i].distFromConsumer;
        cout << "Enter Traffic % for Warehouse " << warehouses[i].id << " (0-100): ";
        cin >> warehouses[i].trafficPercent;
    }

    // --- 3. Algorithm Logic ---
    
    // Step A: Find Nearest Partner
    sort(partners.begin(), partners.end(), comparePartners);
    Partner bestPartner = partners[0];
    
    cout << "\n[LOG] Nearest Partner Selected: " << bestPartner.id 
         << " (Distance: " << bestPartner.distToConsumer << ")" << endl;

    // Step B: Calculate Best Warehouse based on Traffic
    double minEffectiveCost = numeric_limits<double>::max();
    Warehouse bestWarehouse;
    double finalTotalDistance = 0;

    cout << "\n[LOG] Analyzing Warehouse Routes..." << endl;

    for(const auto &w : warehouses) {
        // Formula: Dist * (1 + Traffic/100)
        double trafficFactor = 1.0 + (w.trafficPercent / 100.0);
        double returnCost = w.distFromConsumer * trafficFactor;
        
        double totalCost = bestPartner.distToConsumer + returnCost;
        
        cout << " -> Warehouse " << w.id << " Score: " << totalCost << endl;
        
        if(totalCost < minEffectiveCost) {
            minEffectiveCost = totalCost;
            bestWarehouse = w;
            finalTotalDistance = bestPartner.distToConsumer + w.distFromConsumer;
        }
    }

    // --- 4. Final Output ---
    cout << "\n=========================================" << endl;
    cout << " FINAL ROUTING INSTRUCTION " << endl;
    cout << "=========================================" << endl;
    cout << " Pick Up By Partner: " << bestPartner.id << endl;
    cout << " Deliver To Store:   " << bestWarehouse.id << endl;
    cout << " Total Distance:     " << finalTotalDistance << " km" << endl;
    cout << "=========================================" << endl;

    return 0;
}