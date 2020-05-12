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
void mem_leak3(int n)
{
  F*a=new F(n);
  free(a);
}
void mem_leak3_fix(int n)
{
 
  F*a=new F(n);
 delete a;
}

int main()
{
    mem_leak3(10);
    mem_leak3_fix(9);
     
  return 0;
}