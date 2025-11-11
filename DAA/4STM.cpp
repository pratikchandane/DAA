#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>
#include <iomanip>
using namespace std;

#define INF 1e9 // A large value representing infinity

// Dijkstra’s Algorithm Function
void dijkstra(int V, vector<vector<pair<int, int>>> &adj, int source, vector<int> &parent) {
    vector<int> dist(V, INF);
    dist[source] = 0;

    // Min-heap for finding the shortest distance vertex
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u])
            continue;

        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            // Relaxation step
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Store parent to trace path
                pq.push({dist[v], v});
            }
        }
    }

    cout << "\n Shortest travel times from source (" << source << "):\n";
    for (int i = 0; i < V; i++) {
        cout << "To Intersection " << setw(2) << i << " : ";
        if (dist[i] == INF)
            cout << "No Path\n";
        else
            cout << dist[i] << " minutes\n";
    }
}

// Function to print the shortest path from source to destination
void printPath(int destination, vector<int> &parent) {
    stack<int> path;
    int node = destination;

    while (node != -1) {
        path.push(node);
        node = parent[node];
    }

    cout << "\n Optimal Path to Hospital (Destination " << destination << "): ";
    while (!path.empty()) {
        cout << path.top();
        path.pop();
        if (!path.empty())
            cout << " -> ";
    }
    cout << endl;
}

// Function to update traffic weights dynamically
void updateTraffic(vector<vector<pair<int, int>>> &adj, int u, int v, int newWeight) {
    for (auto &edge : adj[u]) {
        if (edge.first == v) {
            edge.second = newWeight;
            break;
        }
    }
    for (auto &edge : adj[v]) { // Undirected graph update
        if (edge.first == u) {
            edge.second = newWeight;
            break;
        }
    }
}

int main() {
    int V = 7; // Number of intersections (nodes)
    vector<vector<pair<int, int>>> adj(V);

    // Adding roads (edges) with travel times (weights)
    adj[0].push_back({1, 4});
    adj[0].push_back({2, 2});
    adj[1].push_back({2, 5});
    adj[1].push_back({3, 10});
    adj[2].push_back({4, 3});
    adj[4].push_back({3, 4});
    adj[3].push_back({5, 11});
    adj[4].push_back({6, 6});
    adj[5].push_back({6, 3});

    int source = 0;         // Ambulance starting point
    int destination = 5;    // Hospital location

    vector<int> parent(V, -1); // To store shortest path tree

    // Run Dijkstra’s Algorithm
    dijkstra(V, adj, source, parent);
    printPath(destination, parent);

    // Simulate a real-time traffic update
 
    // Re-run Dijkstra after update


    return 0;
}

