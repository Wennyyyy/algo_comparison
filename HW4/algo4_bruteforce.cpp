#include <time.h>
#include <windows.h>

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
    if (argc != 2) {
        cout << "ERROR!!ROAR!!" << endl;
        return 1;
    }
    ifstream fin;
    ofstream fout;
    fin.open(argv[1], ios::in);
    if (!fin.is_open()) {
        cout << "Failed to open" << argv[1] << "!!! U idiot sandwitch" << endl;
        return 1;
    }

    int nCities, solutionCost = 0;
    fin >> nCities;
    vector<vector<int>> Cost(nCities, vector<int>(nCities));
    for (int i = 0; i < nCities; i++) {
        for (int j = 0; j < nCities; j++) {
            fin >> Cost[i][j];
        }
    }
    fin.close();

    clock_t startTime, endTime;
    int s = 0;
    startTime = clock();
    Sleep(1000);
    solutionCost = TSP(Cost, nCities, s);
    endTime = clock();
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
    cout << "Time: " << ((double)(endTime - startTime)) / CLOCKS_PER_SEC - 1 << " s\n";
    return 0;
}