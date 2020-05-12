#include<iostream>
using namespace std;

void mem_leak2(int n)
{
  int*p;
  p=(int*)malloc(sizeof(int)*n);
}
void mem_leak2_fix(int n)
{
  int*q;
  q=(int*)malloc(sizeof(int)*n);
  free(q);
}
int main()
{
   
     mem_leak2(10);
    mem_leak2_fix(9);
  return 0;
}