#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <chrono> 
#include <map>    

using namespace std;
using namespace std::chrono; 

vector<unordered_set<int>> graph;

int maximalCliqueCount = 0;

map<int, int> cliqueSizeDistribution;
int largestCliqueSize = 0;

vector<int> computeDegeneracyOrder(int &degeneracy) {
    int n = graph.size();
    vector<int> degree(n);
    vector<int> order;
    order.reserve(n);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    for (int v = 0; v < n; v++) {
        degree[v] = graph[v].size();
        pq.emplace(degree[v], v);
    }

    degeneracy = 0;
    vector<bool> processed(n, false);

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (processed[v] || degree[v] != d) continue;

        degeneracy = max(degeneracy, d);
        order.push_back(v);
        processed[v] = true;

        for (int neighbor : graph[v]) {
            if (!processed[neighbor]) {
                degree[neighbor]--;
                pq.emplace(degree[neighbor], neighbor);
            }
        }
    }

    reverse(order.begin(), order.end());
    return order;
}

void BronKerboschPivot(unordered_set<int> &P, unordered_set<int> &R, unordered_set<int> &X) {
    if (P.empty() && X.empty()) {
        maximalCliqueCount++;
        int cliqueSize = R.size();
        
        cliqueSizeDistribution[cliqueSize]++;
        
        largestCliqueSize = max(largestCliqueSize, cliqueSize);

        return;
    }

    int pivot = -1, maxNeighbors = -1;

    for (int u : X) {
        int count = 0;
        for (int v : P) {
            if (graph[u].count(v)) count++;
        }
        if (count > maxNeighbors) {
            maxNeighbors = count;
            pivot = u;
        }
    }

    if (pivot == -1) {
        for (int u : P) {
            int count = 0;
            for (int v : P) {
                if (graph[u].count(v)) count++;
            }
            if (count > maxNeighbors) {
                maxNeighbors = count;
                pivot = u;
            }
        }
    }

    vector<int> candidates;
    for (int v : P) {
        if (pivot == -1 || !graph[pivot].count(v)) {
            candidates.push_back(v);
        }
    }

    for (int v : candidates) {
        R.insert(v);
        unordered_set<int> newP, newX;

        for (int u : P) {
            if (graph[v].count(u)) newP.insert(u);
        }

        for (int u : X) {
            if (graph[v].count(u)) newX.insert(u);
        }

        BronKerboschPivot(newP, R, newX);

        R.erase(v);
        P.erase(v);
        X.insert(v);
    }
}

void BronKerboschDegeneracy() {
    int degeneracy;
    vector<int> order = computeDegeneracyOrder(degeneracy);
    vector<int> vertexPosition(graph.size());

    for (size_t i = 0; i < order.size(); i++) {
        vertexPosition[order[i]] = i;
    }

    for (int vi : order) {
        unordered_set<int> P, X;
        for (int neighbor : graph[vi]) {
            if (vertexPosition[neighbor] > vertexPosition[vi]) {
                P.insert(neighbor);
            } else {
                X.insert(neighbor);
            }
        }

        unordered_set<int> R = {vi};
        BronKerboschPivot(P, R, X);
    }
}

void readGraph(const string &filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }

    string line;

    while (getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;
        break;
    }


    

    int u, v;
    unordered_map<int, int> vertexMapping;
    vector<pair<int, int>> edges;
    unordered_set<int> uniqueVertices;

    while (infile >> u >> v) {
        uniqueVertices.insert(u);
        uniqueVertices.insert(v);
        edges.emplace_back(u, v);
    }

    infile.close();

    int index = 0;
    for (int vertex : uniqueVertices) {
        vertexMapping[vertex] = index++;
    }

    graph.resize(uniqueVertices.size());

    for (const auto &[origU, origV] : edges) {
        int newU = vertexMapping[origU];
        int newV = vertexMapping[origV];

        graph[newU].insert(newV);
        graph[newV].insert(newU);  
    }

    cout << "Graph loaded: " << graph.size() << " vertices, " << edges.size() << " edges" << endl;
}

int main() {
    string filename = "Email-Enron.txt";

    auto start = high_resolution_clock::now();

    readGraph(filename);
    BronKerboschDegeneracy();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Number of maximal cliques: " << maximalCliqueCount << endl;
    cout << "Time to find maximal cliques: " << duration.count() << " ms" << endl;
    cout << "Clique size distribution:" << endl;
    cout << "Size\tCount" << endl;
    cout << "----------------" << endl;
    
    for (const auto& [size, count] : cliqueSizeDistribution) {
        cout << size << "\t" << count << endl;
    }
    
    cout << "Total number of cliques: " << maximalCliqueCount << endl;
    cout << "Largest maximal clique size: " << largestCliqueSize << endl;

    return 0;
}