#include <cstdio>
#include <cstdlib>
#include <vector>

int main() {
  int n = 50;
  int range = 5;
  std::vector<int> key(n);
#pragma omp parallel for
  for (int i=0; i<n; i++) {
    key[i] = rand() % range;
    printf("%d ",key[i]);
  }
  printf("\n");

  std::vector<int> bucket(range,0); 
#pragma omp parallel for
  for (int i=0; i<n; i++)
#pragma omp atomic update
    bucket[key[i]]++;
  std::vector<int> offset(range,0);

//---> Failed to do "Parallel Prefix Sum"
//  std::vector<int> otw(range,0);
//#pragma omp parallel
//  for (int j=1; j<range; j<<=1){
//#pragma omp for
//    for (int i=0; i<range; i++)
//      otw[i] = offset[i];
//#pragma omp for
//    for (int i=j; i<range; i++)
//      offset[i] += otw[i-j];
//  }

  for (int i=1; i<range; i++)
    offset[i] = offset[i-1] + bucket[i-1];

#pragma omp parallel for
  for (int i=0; i<range; i++) {
    int j = offset[i];
    for (; bucket[i]>0; bucket[i]--){
      key[j++] = i;
    }
  }
#pragma omp parallel
  for (int i=0; i<n; i++) {
#pragma omp single
    printf("%d ",key[i]);
  }
  printf("\n");
}
