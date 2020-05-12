#include<iostream>
using namespace std;
class F
{
  public:
  int*x;
  F(int n)
  {
    x=new int[n];
  }
  ~F()
  {
    delete []x;
  }
};
void mem_leak5(int n)
{
 void*a=new F(n);
 delete a;
}
void mem_leak5_fix(int n)
{
 
  F*a=new F(n);
   delete a;
}

int main()
{
    mem_leak5(10);
    mem_leak5_fix(9);
     
  return 0;
}