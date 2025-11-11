#include <bits/stdc++.h>
using namespace std;



struct Edge {
    int to;
    double weight;
};

struct Graph {
    int V; // total nodes
    int stages; // number of stages (N)
    vector<int> stageOf; // stageOf[node] -> 0..N-1
    // adjacency: for each node u, list of edges u -> v (v must be in next stage)
    vector<vector<Edge>> adj;

    // constructor
    Graph(int V_, int stages_): V(V_), stages(stages_) {
        stageOf.assign(V, -1);
        adj.assign(V, {});
    }

    void setStage(int node, int stage) {
        if (stage < 0 || stage >= stages) {
            cerr << "Invalid stage\n";
            return;
        }
        stageOf[node] = stage;
    }

    // add (directed) edge u -> v with weight w
    void addEdge(int u, int v, double w) {
        if (u < 0 || u >= V || v < 0 || v >= V) {
            cerr << "Invalid node indices\n";
            return;
        }
        if (stageOf[u] == -1 || stageOf[v] == -1) {
            cerr << "Stage not set for some node\n";
            return;
        }
        // Optional: enforce edges only to next stage
        if (stageOf[v] != stageOf[u] + 1) {
            cerr << "Warning: Edge " << u << "->" << v << " not between consecutive stages (" 
                 << stageOf[u] << " -> " << stageOf[v] << ").\n";
            // We still allow it but algorithm assumes stage-by-stage edges.
        }
        adj[u].push_back({v, w});
    }

    // update edge weight for first matching edge u->v (for multiedges this updates first found)
    bool updateEdge(int u, int v, double newW) {
        if (u < 0 || u >= V) return false;
        bool updated = false;
        for (auto &e : adj[u]) {
            if (e.to == v) {
                e.weight = newW;
                updated = true;
                break; // update only first occurrence; extendable to all
            }
        }
        return updated;
    }

    // Core: compute shortest path from source (must be in stage 0) to dest (must be in stage N-1)
    // Uses DP stage-by-stage (O(E) time). Returns pair {cost, path vector}
    pair<double, vector<int>> computeOptimalPath(int source, int dest) {
        if (source < 0 || source >= V || dest < 0 || dest >= V) {
            return {numeric_limits<double>::infinity(), {}};
        }
        if (stageOf[source] != 0 || stageOf[dest] != stages - 1) {
            // It's still possible if user wants arbitrary nodes, but we enforce typical staged usage
            // We allow it but warn:
            // return {inf, {}};
        }

        // dpCost for nodes in current stage
        const double INF = numeric_limits<double>::infinity();
        vector<double> dpCost(V, INF);
        vector<int> parent(V, -1);

        
        dpCost[source] = 0.0;

        // iterate stages 0 -> stages-1
        for (int s = 0; s < stages - 1; ++s) {
            // For each node u in stage s with finite dpCost, relax its outgoing edges
            vector<double> nextCost(V, INF);
            vector<int> nextParent = parent; // we'll update parents when we improve

            for (int u = 0; u < V; ++u) {
                if (stageOf[u] != s) continue;
                if (dpCost[u] == INF) continue;
                for (auto &e : adj[u]) {
                    int v = e.to;
                    double w = e.weight;
                    // optional: enforce that v is in s+1 (or later); typical model expects s+1
                    if (dpCost[u] + w < nextCost[v]) {
                        nextCost[v] = dpCost[u] + w;
                        nextParent[v] = u;
                    }
                }
            }

            // move nextCost into dpCost for nodes in next stage (but keep INF for others)
            // We can copy only nodes belonging to next stage for efficiency
            for (int v = 0; v < V; ++v) {
                if (stageOf[v] == s + 1) {
                    dpCost[v] = nextCost[v];
                } else {
                    // nodes in later stages retain INF until we reach their stage
                    dpCost[v] = INF;
                }
            }
            parent = nextParent; // update parent tracking
        }

        double finalCost = dpCost[dest];
        vector<int> path;
        if (finalCost == INF) {
            return {INF, path}; // no feasible path
        }

        // reconstruct path using parent[] starting from dest
        int cur = dest;
        while (cur != -1) {
            path.push_back(cur);
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());
        return {finalCost, path};
    }

    vector<pair<double, vector<int>>> batchProcess(const vector<pair<int,int>>& queries) {
        vector<pair<double, vector<int>>> results;
        results.reserve(queries.size());
        for (auto &q : queries) {
            results.push_back(computeOptimalPath(q.first, q.second));
        }
        return results;
    }
};


int main() {
    
    int V = 9;
    int stages = 4;
    Graph G(V, stages);

    // set stage for each node
    G.setStage(0, 0);
    G.setStage(1, 0);
    G.setStage(2, 1);
    G.setStage(3, 1);
    G.setStage(4, 2);
    G.setStage(5, 2);
    G.setStage(6, 3);
    G.setStage(7, 3);
    G.setStage(8, 3);

    G.addEdge(0, 2, 5.0);
    G.addEdge(0, 3, 6.0);
    G.addEdge(1, 2, 4.0);
    G.addEdge(1, 3, 2.0);

    // from stage 1 to 2
    G.addEdge(2, 4, 3.0);
    G.addEdge(2, 5, 7.0);
    G.addEdge(3, 4, 4.5);
    G.addEdge(3, 5, 1.5);

    // stage 2 to 3 (destinations)
    G.addEdge(4, 6, 6.0);
    G.addEdge(4, 7, 4.0);
    G.addEdge(5, 7, 2.0);
    G.addEdge(5, 8, 3.0);

    
    auto result = G.computeOptimalPath(0, 8);
    if (isfinite(result.first)) {
        cout << "Best cost/time = " << result.first << "\nPath: ";
        for (int n : result.second) cout << n << (n == result.second.back() ? "\n" : " -> ");
    } else {
        cout << "No feasible path from source to destination.\n";
    }

    
    vector<pair<int,int>> queries = { {0,6}, {0,7}, {0,8} };
    auto batchResults = G.batchProcess(queries);
    for (size_t i = 0; i < queries.size(); ++i) {
        cout << "Query: " << queries[i].first << " -> " << queries[i].second << "\n";
        if (isfinite(batchResults[i].first)) {
            cout << "  Cost = " << batchResults[i].first << " Path: ";
            for (int n : batchResults[i].second) cout << n << (n == batchResults[i].second.back() ? "\n" : " -> ");
        } else {
            cout << "  No feasible path\n";
        }
    }

    
   

    return 0;
}

