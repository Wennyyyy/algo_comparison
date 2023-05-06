#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
#define INF 5000
// nmax:15 Cmax:100
vector<int> solution;

int TSP(vector<vector<int>> &map, int n, int s) {
    vector<int> possibleWay;
    vector<int> vertexes;
    int minPath = INF;
    int currentPath = 0, vtx = s;

    for (int i = 0; i < n; i++) {
        if (i != s) {
            // store other vertexes except for city 1
            vertexes.push_back(i);
        }
    }

    do {
        // clear possible path
        possibleWay.erase(possibleWay.begin(), possibleWay.end());
        currentPath = 0;
        vtx = s;
        for (int i = 0; i < vertexes.size(); i++) {
            currentPath += map[vtx][vertexes[i]];
            vtx = vertexes[i];
            possibleWay.push_back(vtx);
        }
        currentPath += map[vtx][s];
        if (minPath > currentPath) {
            solution.erase(solution.begin(), solution.end());
            for (int i = 0; i < possibleWay.size(); i++) {
                solution.push_back(possibleWay[i]);
            }
            minPath = currentPath;

            // first time must updated through ascending order(1 ~ n -> 1)
            /* cout << "Updated Solution: 1 -> ";
            for(int i = 0; i < solution.size(); i++){
                cout << solution[i] + 1;
                if( i < solution.size() - 1){
                cout << " -> " ;
                }else{
                cout <<" -> 1" <<endl;
                }
            }*/
        }
        // do for all permutation
    } while (next_permutation(vertexes.begin(), vertexes.end()));
    return minPath;
}
int main(int argc, char *argv[]) {
    int nCities, solutionCost = 0;
    cin >> nCities;
    vector<vector<int>> Cost(nCities, vector<int>(nCities));
    for (int i = 0; i < nCities; i++) {
        for (int j = 0; j < nCities; j++) {
            cin >> Cost[i][j];
        }
    }

#ifdef _WIN32
    LARGE_INTEGER start, end, tc;
#else
    struct timeval start, end;
#endif
    int s = 0;
    double time = 0;

#ifdef _WIN32
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&start);
#else
    gettimeofday(&start, NULL);
#endif

    solutionCost = TSP(Cost, nCities, s);

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    time = (double)(end.QuadPart - start.QuadPart) / (double)tc.QuadPart;
#else
    gettimeofday(&end, NULL);
    time = (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;
#endif

    cout << "The shortest Hamiltonian cycle:" << endl
         << "1 ";
    for (int i = 0; i < solution.size(); i++) {
        cout << solution[i] + 1;
        if (i < solution.size() - 1) {
            cout << " ";
        } else {
            cout << " 1" << endl;
        }
    }
    cout << "Total cost: " << solutionCost << endl;
    cout << "Time: " << time << endl;

    return 0;
}