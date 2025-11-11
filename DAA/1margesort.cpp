#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

// Function to convert timestamp (hh:mm AM/PM) to 24-hour format in minutes
int convertToMinutes(string timeStr) {
    int hours, minutes;
    char colon;
    string meridian; // AM or PM
    stringstream ss(timeStr);
    ss >> hours >> colon >> minutes >> meridian;

    // Convert to 24-hour format
    if (meridian == "PM" && hours != 12) hours += 12;
    if (meridian == "AM" && hours == 12) hours = 0;

    return hours * 60 + minutes;
}

// Structure to represent customer order
struct Order {
    string customerName;
    string timestamp;
};

// Merge function
void merge(vector<Order>& orders, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Order> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = orders[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = orders[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (convertToMinutes(L[i].timestamp) <= convertToMinutes(R[j].timestamp)) {
            orders[k] = L[i];
            i++;
        } else {
            orders[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        orders[k] = L[i];
        i++; k++;
    }
    while (j < n2) {
        orders[k] = R[j];
        j++; k++;
    }
}

// Merge Sort recursive function
void mergeSort(vector<Order>& orders, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(orders, left, mid);
        mergeSort(orders, mid + 1, right);
        merge(orders, left, mid, right);
    }
}

int main() {
    int n;
    cout << "Enter number of customer orders: ";
    cin >> n;
    cin.ignore();

    vector<Order> orders(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter customer name: ";
        getline(cin, orders[i].customerName);
        cout << "Enter time1;1 (hh:mm AM/PM): ";
        getline(cin, orders[i].timestamp);
    }

    mergeSort(orders, 0, n - 1);

    cout << "\nSorted Orders by Time:\n";
    for (auto &order : orders) {
        cout << order.customerName << " - " << order.timestamp << endl;
    }

    return 0;
}

