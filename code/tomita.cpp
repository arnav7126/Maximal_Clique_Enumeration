#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <map>

using namespace std;

const int MAX_NODES = 2000000;
vector<int> nodeMapping(MAX_NODES, -1);
vector<unordered_set<int>> graph;

unordered_set<int> Q;
int cliqueCount = 0, maxCliqueSize = 0, nodeIndex = 0;
map<int, int> cliqueSizeDistribution;

void loadGraph(const string &filename, int &numNodes, int &numEdges) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << "\n";
        exit(1);
    }

    unordered_map<int, int> nodeMap;
    graph.clear();
    nodeIndex = 0;
    numEdges = 0;

    string line;
    
    while (getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;
        break;
    }

    int u, v;
    do {
        if (sscanf(line.c_str(), "%d %d", &u, &v) != 2) continue;

        if (nodeMap.find(u) == nodeMap.end()) nodeMap[u] = nodeIndex++;
        if (nodeMap.find(v) == nodeMap.end()) nodeMap[v] = nodeIndex++;

        int mappedU = nodeMap[u];
        int mappedV = nodeMap[v];

        if (mappedU != mappedV) {
            if (graph.size() <= max(mappedU, mappedV)) {
                graph.resize(max(mappedU, mappedV) + 1);
            }
            graph[mappedU].insert(mappedV);
            graph[mappedV].insert(mappedU);
        }

        numEdges++;
    } while (getline(infile, line));

    infile.close();
    numNodes = nodeIndex;

    cout << "Graph Loaded! Nodes: " << numNodes << ", Edges: " << numEdges << "\n";
}

void expand(unordered_set<int> &SUBG, unordered_set<int> &CAND) {
    if (SUBG.empty()) {
        cliqueCount++;
        int cliqueSize = Q.size();
        maxCliqueSize = max(maxCliqueSize, cliqueSize);
        cliqueSizeDistribution[cliqueSize]++;
        return;
    }

    int pivot = -1, maxIntersection = -1;
    for (int u : SUBG) {
        int intersectionSize = 0;
        if (CAND.size() < graph[u].size()) {
            for (int v : CAND) {
                if (CAND.find(v) != CAND.end()) {
                    intersectionSize++;
                }
            }
        } else {
            for (int v : graph[u]) {
                if (CAND.find(v) != CAND.end()) {
                    intersectionSize++;
                }
            }
        }
        if (intersectionSize > maxIntersection) {
            maxIntersection = intersectionSize;
            pivot = u;
        }
    }

    for (auto it = CAND.begin(); it != CAND.end();) {
        int q = *it;
        
        if (graph[pivot].find(q) != graph[pivot].end()) {
            ++it;  
            continue;
        }

        Q.insert(q);

        unordered_set<int> nextSUBG, nextCAND;
        for (int v : SUBG) {
            if (graph[q].find(v) != graph[q].end()) {
                nextSUBG.insert(v);
            }
        }
        for (int v : CAND) {
            if (graph[q].find(v) != graph[q].end()) {
                nextCAND.insert(v);
            }
        }

        expand(nextSUBG, nextCAND);

        Q.erase(q);
        it = CAND.erase(it);
    }
}

int main() {
    string filename = "email-Enron.txt";
    int numNodes = 0, numEdges = 0;

    loadGraph(filename, numNodes, numEdges);
    auto start = chrono::high_resolution_clock::now();

    unordered_set<int> CAND, SUBG;
    for (int i = 0; i < numNodes; i++) {
        CAND.insert(i);
        SUBG.insert(i);
    }
    expand(SUBG, CAND);

    auto end = chrono::high_resolution_clock::now();
    long long elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    double elapsed_sec = elapsed_ms / 1000.0;

    cout << "Number of maximal cliques: " << cliqueCount << "\n";
    cout << "Time to find maximal cliques: " << elapsed_sec << " seconds\n\n";

    cout << "Clique size distribution:\n";
    cout << "Size\tCount\n";
    cout << "----------------\n";
    for (const auto &entry : cliqueSizeDistribution) {
        cout << entry.first << "\t" << entry.second << "\n";
    }

    cout << "\nTotal number of cliques: " << cliqueCount << "\n";
    cout << "Largest maximal clique size: " << maxCliqueSize << "\n";

    return 0;
}
