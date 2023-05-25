#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
// swap elements
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}
// rearrangement
int partition(vector<int>&data, int left, int right) {
  int pivot = data[right], i = (left - 1);
  for (int j = left; j < right; j++) {
    if (data[j] < pivot) {
      i++;
      swap(&data[i], &data[j]);
    }
  }
  swap(&data[i + 1], &data[right]);
  return (i + 1);
}
void quickSort(vector<int>&data, int left, int right) {
  if (left < right) {
    int piv = partition(data, left, right);

    // recursive call on the left of pivot
    quickSort(data, left, piv - 1);
    // recursive call on the right of pivot
    quickSort(data, piv + 1, right);
  }
}
int main(int argc, char *argv[]) {
     int n = 0, k = 0, temp = 0;
    cin >> n >> k;
    vector<int> data(n);
    for (int i = 0; i < n; i++) {
        cin >> data[i];
    }
    /*for (auto it = data.begin(); it != data.end(); ++it) {
      cout << *it << " ";
    }*/

#ifdef _WIN32
    LARGE_INTEGER start, end, tc;
#else
    struct timeval start, end;
#endif
    double time = 0;

#ifdef _WIN32
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&start);
#else
    gettimeofday(&start, NULL);
#endif

    quickSort(data, 0, n-1);


#ifdef _WIN32
    QueryPerformanceCounter(&end);
    time = (double)(end.QuadPart - start.QuadPart) / (double)tc.QuadPart;
#else
    gettimeofday(&end, NULL);
    time = (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;
#endif

    cout << "Output: " << endl;
    for(int i = 0; i < k; i++){
      cout << data[i];
      if(i != 0 && i % 10 == 0 ){
        cout << endl;
      }
      else{
        cout << " ";
      }
    }
    cout << endl;
    cout << "Time: " << time << endl;
    return 0;
}