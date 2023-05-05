#include <iostream>
#include <cstdlib>
using namespace std;
#define n 15

int main(int argc, char* argv[]){
    cout << n << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i == j){
                cout << "100" ;
                if( j == n-1){
                }else{
                    cout << " ";
                }
            }else{
                cout << (rand() % 50) + 1 ;
                if( j == n-1){
                }else{
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
    return 0;
}