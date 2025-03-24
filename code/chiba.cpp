#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <chrono>
#include <cstring>

using namespace std;
using namespace std::chrono;


using Graph = map<int, set<int>>;


vector<int> C;                     
vector<int> CV;                    
vector<vector<int>> graph;         
vector<int> S;                    
vector<int> T;                    
vector<int> diffCN;               
vector<int> interCN;              
vector<vector<int>> diffNkC;      
long graph_size = 0;

void buildGraph(Graph &graph_init, const string &filepath)
{
    ifstream fileStream(filepath);
    if (!fileStream.is_open())
    {
        cerr << "Error: Could not open file " << filepath << '\n';
        exit(1);
    }
    string line;
    while (getline(fileStream, line))
    {
        if (line[0] == '#')
            continue;
        istringstream iss(line);
        int u, v;
        iss >> u >> v;
        graph_init[u].insert(v);
        graph_init[v].insert(u);
    }
    fileStream.close();
}

void printResults(int total_maximal_cliques, int largest_clique_size, const map<int, int> &clique_size_distribution, double time_taken)
{
    cout << "\nResults:\n";
    cout << "1. Total Max Cliques: " << total_maximal_cliques << '\n';
    cout << "2. Largest Clique Size: " << largest_clique_size << '\n';
    cout << "3. Time Taken: " << time_taken << " ms\n";
    cout << "4. Distribution of clique sizes: \n";
    for (auto &p : clique_size_distribution)
        cout << "   Size " << p.first << ": " << p.second << " cliques" << '\n';
}

void sort_algo(vector<int> &arr)
{
    int n = arr.size();
    int max_val = *max_element(arr.begin(), arr.end());
    vector<int> count(max_val + 1, 0);
    vector<int> out(n, 0);
    for (int i = 0; i < n; i++)
        count[arr[i]]++;
    for (int i = 1; i <= max_val; i++)
        count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--)
    {
        out[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
    for (int i = 0; i < n; i++)
        arr[i] = out[i];
}

void update(int i, int &total_maximal_cliques, int &largest_clique_size, map<int, int> &clique_size_distribution)
{
    int csize = C.size();
    int neighbor_size = graph[i].size();

    if (i == graph_size + 1)
    {
        total_maximal_cliques++;
        if (csize > largest_clique_size)
            largest_clique_size = csize;
        clique_size_distribution[csize]++;
        return;
    }

    diffCN.clear();
    size_t j = 0, k = 0;
    while (j < csize && k < neighbor_size)
    {
        if (C[j] < graph[i][k])
        {
            diffCN.push_back(C[j]);
            break; 
            ++j;
        }
        else if (graph[i][k] < C[j])
        {
            ++k;
        }
        else
        {
            ++j;
            ++k;
        }
    }
    while (j < csize)
    {
        diffCN.push_back(C[j]);
        break;
        ++j;
    }

    if (!diffCN.empty())
        update(i + 1, total_maximal_cliques, largest_clique_size, clique_size_distribution);

    diffCN.clear();
    interCN.clear();
    csize = C.size();
    j = 0;
    k = 0;
    while (j < csize && k < neighbor_size)
    {
        if (C[j] < graph[i][k])
        {
            diffCN.push_back(C[j]);
            ++j;
        }
        else if (graph[i][k] < C[j])
        {
            ++k;
        }
        else
        {
            interCN.push_back(C[j]);
            ++j;
            ++k;
        }
    }
    while (j < csize)
    {
        diffCN.push_back(C[j]);
        ++j;
    }

    // Optimization: Early pruning
    if (interCN.empty())
    {
        if (!diffCN.empty() && diffCN.back() < (i - 1))
            return;
    }

    memset(T.data(), 0, T.size() * sizeof(int));

    for (int x : interCN)
    {
        for (int y : graph[x])
        {
            if (!CV[y] && y != i)
                T[y]++;
        }
    }
    for (int x : graph[i])
    {
        if (!CV[x])
        {
            if (x > i)
                break;
            if (x < i && T[x] == (int)interCN.size())
                return;
        }
    }

    memset(S.data(), 0, S.size() * sizeof(int));

    diffNkC.clear();
    int p = diffCN.size();
    diffNkC.resize(p);
    int *__restrict S_ptr = S.data();
    int *__restrict CV_ptr = CV.data();
    for (int k = 0; k < p; k++)
    {
        const vector<int> &neighbors = graph[diffCN[k]];
        for (size_t j = 0, n = neighbors.size(); j < n; j++)
        {
            int y = neighbors[j];
            if (!CV_ptr[y])
            {
                ++S_ptr[y];
                if (y < i)
                    diffNkC[k].push_back(y);
            }
        }
    }


    for (int k = 0; k < p; k++)
    {
        int current = diffCN[k];
        int prev = (k == 0 ? 0 : diffCN[k - 1]);
        for (int y : diffNkC[k])
        {
            if (T[y] == (int)interCN.size())
            {
                if (y >= current)
                {
                    S[y]--;
                }
                else
                {
                    bool flag_first_j = false;
                    if (y < current && y >= prev)
                        flag_first_j = true;
                    if (flag_first_j)
                    {
                        if (k == 0 && S[y] == p)
                            return;
                        if (k > 0 && S[y] + k == p && prev <= y)
                            return;
                    }
                }
            }
        }
    }

    if (!interCN.empty())
    {
        for (int y = 1; y < i; y++)
        {
            if (!CV[y] && T[y] == (int)interCN.size() && S[y] == 0)
            {
                if (!diffCN.empty() && diffCN.back() < y)
                    return;
            }
        }
    }
    set<int> save(diffCN.begin(), diffCN.end());

    C.clear();
    memset(CV.data(), 0, CV.size() * sizeof(int));

    for (int x : interCN)
    {
        C.push_back(x);
        CV[x] = 1;
    }
    C.push_back(i);
    CV[i] = 1;

    update(i + 1, total_maximal_cliques, largest_clique_size, clique_size_distribution);

    CV[i] = 0;
    for (int x : C)
    {
        if (x != i)
            save.insert(x);
    }

    C.clear();
    for (int x : save)
    {
        C.push_back(x);
        CV[x] = 1;
    }
}

void clique(int &total_maximal_cliques, int &largest_clique_size, map<int, int> &clique_size_distribution)
{
    C.push_back(1);
    CV[1] = 1;
    cout << "Finding all maximal cliques...\n";
    update(2, total_maximal_cliques, largest_clique_size, clique_size_distribution);
}

int main()
{
    auto start = high_resolution_clock::now();
    setbuf(stdout, NULL);

    string filepath = "wiki-Vote.txt";
    Graph graph_init;
    buildGraph(graph_init, filepath);
    graph_size = graph_init.size();

    cout << "Renumbering vertices..." << endl;
    int max_degree = 0, max_vertex = 0;
    for (auto &v : graph_init)
    {
        max_degree = max(max_degree, (int)v.second.size());
        max_vertex = max(max_vertex, v.first);
    }

    vector<vector<int>> buckets(max_degree + 1);
    for (auto &v : graph_init)
    {
        int degree = v.second.size();
        buckets[degree].push_back(v.first);
    }

    vector<int> old_to_new(max_vertex + 1, -1);
    vector<int> new_to_old(graph_size + 1, -1);
    long new_index = 1;
    for (int d = 0; d <= max_degree; ++d)
    {
        for (int vertex : buckets[d])
        {
            old_to_new[vertex] = new_index;
            new_to_old[new_index] = vertex;
            new_index++;
        }
    }

    graph.resize(graph_size + 1);
    S.resize(graph_size + 1);
    T.resize(graph_size + 1);
    CV.resize(graph_size + 1);
    diffCN.resize(graph_size + 1);
    interCN.resize(graph_size + 1);
    memset(CV.data(), 0, CV.size() * sizeof(int));

    for (auto &v : graph_init)
    {
        int new_vertex = old_to_new[v.first];
        for (int neighbor : v.second)
            graph[new_vertex].push_back(old_to_new[neighbor]);
    }
    for (int i = 1; i <= graph.size(); ++i)
    {
        if (graph[i].size() < 20)
            sort(graph[i].begin(), graph[i].end());
        else
            sort_algo(graph[i]);
    }

    int total_maximal_cliques = 0;
    int largest_clique_size = 0;
    map<int, int> clique_size_distribution;

    clique(total_maximal_cliques, largest_clique_size, clique_size_distribution);

    auto end = high_resolution_clock::now();
    double time_taken = duration_cast<milliseconds>(end - start).count();
    printResults(total_maximal_cliques, largest_clique_size, clique_size_distribution, time_taken);

    return 0;
}