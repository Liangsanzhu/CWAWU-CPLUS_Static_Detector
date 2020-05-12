#include<iostream>
#include<string.h>
using namespace std;
//数组越界
int str1[4]={1,2,3,4};
int str2[3];
void f1(void){
    str2[3]=str1[2];
}

//空指针解引用 
int Fault() {
  int* a;
  a = NULL;
  int b=0;
  *a = 5;
  return 0;
}

int Contrast(){
    int *b;
    b = NULL;
    int tmp = 1;
    b = &tmp;
    *b = 5;
    return 0;
}

//内存泄漏
void mem_leak1(int n)
{
  int*p;
  p=new int;

}
void mem_leak1_fix(int n)
{
  int*p;
  p=new int;
  delete p;
}
void mem_leak2(int n)
{
  new int;
  int*m=new int[n];
  delete m;
}
void mem_leak2_fix(int n)
{
  int*m=new int[n];
  delete []m;
}
 /*未初始化变量*/
void namecpy()
{
  char *name;
  int age = 20;
  strcpy(name, "John");
  printf("%s %d\n", name, age);
}
int main()
{
 
 
  return 0;
}