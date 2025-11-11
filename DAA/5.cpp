#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
using namespace std;

#define INF 1e9  // Representing infinity

// Function to find the minimum cost path in a Multistage Graph
int minCostPath(vector<vector<int>> &graph, int N) {
    vector<int> cost(N, INF);   // cost[i] will store min cost to reach destination from i
    vector<int> path(N, -1);    // To store the next node in optimal path

    cost[N - 1] = 0; // Cost of reaching destination from destination is 0

    // Process nodes from second-last to the first
    for (int i = N - 2; i >= 0; i--) {
        for (int j = i + 1; j < N; j++) {
            if (graph[i][j] != INF && cost[i] > graph[i][j] + cost[j]) {
                cost[i] = graph[i][j] + cost[j];
                path[i] = j;
            }
        }
    }

    // Print minimum cost
    cout << "\n Minimum Delivery Cost from Source to Destination: " << cost[0] << endl;

    // Print optimal path
    cout << "Optimal Delivery Route: ";
    int i = 0;
    cout << i;
    while (path[i] != -1) {
        cout << " -> " << path[i];
        i = path[i];
    }
    cout << endl;

    return cost[0];
}

int main() {
    int N = 8; // Total number of nodes (stages + cities)
    vector<vector<int>> graph(N, vector<int>(N, INF));

    // Define the multistage graph (directed, weighted)
    // graph[i][j] = cost from node i to node j
    graph[0][1] = 1;
    graph[0][2] = 2;
    graph[1][3] = 3;
    graph[1][4] = 4;
    graph[2][3] = 2;
    graph[2][4] = 6;
    graph[3][5] = 1;
    graph[3][6] = 2;
    graph[4][6] = 2;
    graph[5][7] = 4;
    graph[6][7] = 3;

    
    cout << "SwiftCargo Delivery Route Optimizer\n";
    

    minCostPath(graph, N);

    return 0;
}

