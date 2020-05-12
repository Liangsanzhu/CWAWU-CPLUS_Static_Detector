#include<iostream>
using namespace std;

void mem_leak4(int n)
{
  int*p=new int[n];
}
void mem_leak4_fix(int n)
{
  int*p=new int[n];
  delete []p;
}
int main()
{
    mem_leak4(10);
    mem_leak4_fix(9);
  return 0;
}