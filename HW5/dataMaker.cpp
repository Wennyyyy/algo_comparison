#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;
// 𝑛 is from 10,000,000 to 30,000,000 in steps of 1,000,000.
// 1-1, 1-2, 1-3, 2-1, ...
// 19 -> 10 20 -> 11
// 29 -> 20
#define n 10000000
#define k 20
int main(int argc, char* argv[]){
    ofstream ofs;
    ofs.open(".\\data\\data1-1.txt");
    if (!ofs.is_open()) {
        cout << "Failed to open file.\n";
        return -1; // EXIT_FAILURE
    }
    ofs << n <<" "<< k << endl;
    for(int i = 0; i < n; i++){
        ofs << (rand() % 1010) + 1 <<" ";
    }
    ofs.close();
    return 0;
}