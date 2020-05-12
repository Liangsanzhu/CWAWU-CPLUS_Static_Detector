#include<iostream>
using namespace std;
void mem_leak1(int n)
{
  new int;
  int*p;
  p=new int[n];
}
void mem_leak1_fix(int n)
{
  int*q;
  q=new int[n];
  delete []q;
}

int main()
{
    mem_leak1(10);
    mem_leak1_fix(9);
     
  return 0;
}