#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

const int INF = INT_MAX;

// Node to represent each state in the search tree
class Node {
public:
    int level;               // Current level (number of cities visited)
    int pathCost;            // Cost of the current path
    int reducedCost;         // Total reduced cost (cost of the path + matrix reduction cost)
    int vertex;              // Current city (vertex)
    vector<int> path;       // Path of cities visited so far
    vector<vector<int>> reducedMatrix;  // Reduced cost matrix after row/column reduction

    Node(int n) {
        reducedMatrix = vector<vector<int>>(n, vector<int>(n, INF));
    }

    bool operator>(const Node& other) const {
        return this->reducedCost > other.reducedCost;
    }
};

// Function to copy matrix
void copyMatrix(const vector<vector<int>>& src, vector<vector<int>>& dest, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// Function to reduce matrix by row and column
int reduceMatrix(vector<vector<int>>& matrix, int n) {
    int reductionCost = 0;

    // Row reduction
    for (int i = 0; i < n; i++) {
        int rowMin = INF;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < rowMin)
                rowMin = matrix[i][j];
        }

        if (rowMin != INF && rowMin != 0) {
            reductionCost += rowMin;
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] != INF)
                    matrix[i][j] -= rowMin;
            }
        }
    }

    // Column reduction
    for (int j = 0; j < n; j++) {
        int colMin = INF;
        for (int i = 0; i < n; i++) {
            if (matrix[i][j] < colMin)
                colMin = matrix[i][j];
        }

        if (colMin != INF && colMin != 0) {
            reductionCost += colMin;
            for (int i = 0; i < n; i++) {
                if (matrix[i][j] != INF)
                    matrix[i][j] -= colMin;
            }
        }
    }
    return reductionCost;
}

// Function to create a new node
Node createNode(const vector<vector<int>>& parentMatrix, const vector<int>& path, int level, int i, int j, int n) {
    Node node(n);
    copyMatrix(parentMatrix, node.reducedMatrix, n);

    // Remove row and column based on the current path
    for (int k = 0; level != 0 && k < n; k++) {
        node.reducedMatrix[i][k] = INF;
    }

    for (int k = 0; k < n; k++) {
        node.reducedMatrix[k][j] = INF;
    }

    if (level + 1 < n)
        node.reducedMatrix[j][0] = INF;

    // Copy the current path and add the new city
    node.path = path;
    node.path.push_back(j);
    node.level = level;
    node.vertex = j;
    return node;
}

// Function to solve the TSP using Branch and Bound
void solveTSP(const vector<vector<int>>& costMatrix, int n) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    vector<int> path;
    path.push_back(0);

    Node root(n);
    copyMatrix(costMatrix, root.reducedMatrix, n);

    root.path = path;
    root.level = 0;
    root.vertex = 0;
    root.pathCost = 0;
    root.reducedCost = reduceMatrix(root.reducedMatrix, n);

    pq.push(root);
    int minCost = INF;
    vector<int> finalPath;

    while (!pq.empty()) {
        Node min = pq.top();
        pq.pop();

        int i = min.vertex;

        // If all cities are visited, return to the start city
        if (min.level == n - 1) {
            min.path.push_back(0);
            int totalCost = min.pathCost + costMatrix[i][0];
            if (totalCost < minCost) {
                minCost = totalCost;
                finalPath = min.path;
            }
            continue;
        }

        // Expand all unvisited cities
        for (int j = 0; j < n; j++) {
            if (min.reducedMatrix[i][j] != INF) {
                Node child = createNode(min.reducedMatrix, min.path, min.level + 1, i, j, n);
                child.pathCost = min.pathCost + costMatrix[i][j];
                child.reducedCost = child.pathCost + reduceMatrix(child.reducedMatrix, n);
                pq.push(child);
            }
        }
    }

    // Output the optimal path and its cost
    cout << "\nOptimal Delivery Route (SwiftShip): ";
    for (int city : finalPath) {
        cout << city + 1 << " ";
    }
    cout << endl;
    cout << "Minimum Total Delivery Cost: " << minCost << endl;
}

int main() {
    int n;
    cout << "Enter number of cities: ";
    cin >> n;

    vector<vector<int>> costMatrix(n, vector<int>(n));

    cout << "Enter cost matrix (use a large number for no direct route):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> costMatrix[i][j];
        }
    }

    solveTSP(costMatrix, n);

    return 0;
}
