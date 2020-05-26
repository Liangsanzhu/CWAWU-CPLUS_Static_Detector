#include<iostream>
using namespace std;
void mem_leak1(int n)
{
  new int;
  int*p=NULL;
  if(n!=0)
  {
    p=new int;
  }
  else if(n==1)
  {
    int*q=(int*)malloc(sizeof(int)*n);
  }
  else if(n==2)
  {
    int*u=new int[n];
  }
  
  
 
  /*
  for(int i=0;i<9;i++)
    {
      int*y=new int[i];
    }
    
    switch (n)
    {
    case 1:
    int*o=new int;
      break;
   
    }
    while(n!=0)
    {
      int*u=new int;
    }*/

}
