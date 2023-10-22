#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

struct Edge {
    int destination;
    int year;
    int time;
    int cost;
};

int main() {
    int N, M;
    cin >> N >> M;

    vector<vector<Edge>> graph(N + 1); // +1 for 1-based indexing

    for (int i = 0; i < M; i++) {
        int u, v, year, time, cost;
        cin >> u >> v >> year >> time >> cost;

        graph[u].push_back({v, year, time, cost});
        graph[v].push_back({u, year, time, cost}); // Include the reverse edge for undirected graph
    }

    vector<int> minTime(N + 1, INT_MAX);
    minTime[1] = 0;
    priority_queue<pair<int, int>> pq; // {time, village}
    pq.push({0, 1});

    while (!pq.empty()) {
        int currTime = -pq.top().first;
        int currVillage = pq.top().second;
        pq.pop();

        for (const Edge& edge : graph[currVillage]) {
            int nextVillage = edge.destination;
            int nextTime = currTime + edge.time;
            if (nextTime < minTime[nextVillage]) {
                minTime[nextVillage] = nextTime;
                pq.push({-nextTime, nextVillage});
            }
        }
    }

    int firstYear = 0;
    for (const Edge& edge : graph[1]) {
        if (minTime[edge.destination] != INT_MAX) {
            firstYear = max(firstYear, edge.year);
        }
    }

    vector<int> minYear(N + 1, INT_MAX);
    minYear[1] = 0;
    pq.push({0, 1}); // Reset priority queue

    while (!pq.empty()) {
        int currYear = -pq.top().first;
        int currVillage = pq.top().second;
        pq.pop();

        for (const Edge& edge : graph[currVillage]) {
            int nextVillage = edge.destination;
            int nextYear = max(currYear, edge.year);
            if (nextYear < minYear[nextVillage]) {
                minYear[nextVillage] = nextYear;
                pq.push({-nextYear, nextVillage});
            }
        }
    }

    vector<bool> visited(N + 1, false);
    vector<Edge> mst; // Minimum Spanning Tree

    pq.push({0, 1}); // Reset priority queue

    while (!pq.empty()) {
        int currCost = -pq.top().first;
        int currVillage = pq.top().second;
        pq.pop();

        if (visited[currVillage]) {
            continue;
        }
        visited[currVillage] = true;

        for (const Edge& edge : graph[currVillage]) {
            if (!visited[edge.destination]) {
                pq.push({-edge.cost, edge.destination});
                mst.push_back(edge);
            }
        }
    }

    int totalCost = 0;
    for (const Edge& edge : mst) {
        totalCost += edge.cost;
    }

    for (int i = 2; i <= N; i++) {
        cout << minTime[i] << endl;
    }
    cout << firstYear << endl;
    cout << minYear[N] << endl;
    cout << totalCost << endl;

    return 0;
}
