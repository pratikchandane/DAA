#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Structure to represent each relief item
struct Item {
    string name;
    float weight;
    float value;
};

// Function to calculate maximum utility using Fractional Knapsack
float fractionalKnapsack(float capacity, vector<Item>& items) {
    // Calculate value-to-weight ratio and sort in descending order
    sort(items.begin(), items.end(), [](Item a, Item b) {
        return (a.value / a.weight) > (b.value / b.weight);
    });

    float totalValue = 0.0;  // total utility value
    float currentWeight = 0.0;

    cout << "\nItems taken in the boat:\n";
    

    for (auto& item : items) {
        if (currentWeight + item.weight <= capacity) {
            // Take the whole item
            currentWeight += item.weight;
            totalValue += item.value;
            cout << "Taken: " << item.name << " (100%)"
                 << " | Weight: " << item.weight
                 << " | Value: " << item.value << endl;
        } 
        else {
            // Take only a fraction of the item
            float remain = capacity - currentWeight;
            float fraction = remain / item.weight;
            totalValue += item.value * fraction;
            currentWeight += remain;
            cout << "Taken: " << item.name << " (" << fraction * 100 << "%)"
                 << " | Weight: " << remain
                 << " | Value: " << item.value * fraction << endl;
            break;
        }
    }

    
    return totalValue;
}

int main() {
    float capacity;
    cout << "Enter boat weight capacity (in kg): ";
    cin >> capacity;

    vector<Item> items = {
        {"Food Packets", 40, 120},
        {"Water Bottles", 10, 30},
        {"Medical Kit", 20, 100},
        {"Blankets", 30, 60},
        {"First Aid Box", 15, 70}
    };

    float maxValue = fractionalKnapsack(capacity, items);

    cout << "\nMaximum Utility Value that can be carried: " << maxValue << endl;
    return 0;
}

