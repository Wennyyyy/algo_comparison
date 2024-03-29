#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <cstring>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
#define N 15
#define MAX 100

struct Node {
    vector<pair<int, int> > path;
    int matrix[N][N];
    int cost;
    int index;
    int level;
};

Node *newNode(int n, int parentMatrix[N][N], vector<pair<int, int> > const &path, int level, int i, int j) {
    Node *node = new Node;

    node->level = level;
    node->index = j;

    node->path = path;
    node->path.push_back(make_pair(i, j));

    memcpy(node->matrix, parentMatrix, sizeof node->matrix);
    for (int k = 0; level != 0 && k < n; k++) {
        node->matrix[i][k] = MAX;
        node->matrix[k][j] = MAX;
    }
    node->matrix[j][0] = MAX;

    return node;
}

int calLowerBound(int n, int matrix[N][N]) {
    int rowMin[n];
    // initial each row's minimim with 100
    for (int i = 0; i < n; i++) rowMin[i] = MAX;
    // find minimim for each row
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < rowMin[i]) {
                rowMin[i] = matrix[i][j];
            }
        }
    }

    // subtract the minimum
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != MAX && rowMin[i] != MAX) {
                matrix[i][j] -= rowMin[i];
            }
        }
    }

    int colMin[n];
    // initial each column's minimim with 100
    for (int i = 0; i < n; i++) colMin[i] = MAX;

    // find minimim for each rcolumn
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < colMin[i]) {
                colMin[i] = matrix[i][j];
            }
        }
    }

    // subtract the minimum
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != MAX && colMin[j] != MAX) {
                matrix[i][j] -= colMin[j];
            }
        }
    }

    // sum the cost
    int cost = 0;
    for (int i = 0; i < n; i++) {
        if (rowMin[i] != MAX) cost += rowMin[i];
        if (colMin[i] != MAX) cost += colMin[i];
    }

    return cost;
}

struct cmp {
    bool operator()(const Node *a, const Node *b) const {
        return a->cost > b->cost;
    }
};

int main(int argc, char *argv[]) {
#ifdef _WIN32
    LARGE_INTEGER start, end, tc;
#else
    struct timeval start, end;
#endif
    int n;
    int totalCost = 0;
    int matrix[N][N];
    double time;
    vector<pair<int, int> > path;

    // store all nodes and sorted from the less cost
    priority_queue<Node *, vector<Node *>, cmp> nodes_q;

    cin >> n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }
#ifdef _WIN32
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&start);
#else
    gettimeofday(&start, NULL);
#endif
    // starts from Node 0
    Node *root = new Node;
    root->level = 0;
    root->index = 0;
    root->path = path;
    memcpy(root->matrix, matrix, sizeof root->matrix);
    root->matrix[0][0] = MAX;
    root->cost = calLowerBound(n, root->matrix);
    nodes_q.push(root);

    while (!nodes_q.empty()) {
        Node *minCostNode = nodes_q.top();
        nodes_q.pop();

        // if all cities are visited
        if (minCostNode->level == n - 1) {
            minCostNode->path.push_back(make_pair(minCostNode->index, 0));
            // print the path
            cout << "The shortest Hamiltonian cycle: \n";
            for (int i = 0; i < minCostNode->path.size(); i++) {
                cout << minCostNode->path[i].first + 1 << " ";
            }
            cout << minCostNode->path[n - 1].second + 1 << "\n";
            totalCost = minCostNode->cost;
            break;
        }

        for (int j = 0; j < n; j++) {
            if (minCostNode->matrix[minCostNode->index][j] != MAX) {
                Node *child = newNode(n, minCostNode->matrix, minCostNode->path, minCostNode->level + 1, minCostNode->index, j);
                child->cost = minCostNode->cost + minCostNode->matrix[minCostNode->index][j] + calLowerBound(n, child->matrix);
                nodes_q.push(child);
            }
        }
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    time = (double)(end.QuadPart - start.QuadPart) / (double)tc.QuadPart;
#else
    gettimeofday(&end, NULL);
    time = (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;
#endif

    cout << "Total cost: " << totalCost << endl;
    cout << "Time: " << time << endl;

    return 0;
}
