#include<iostream>
#include<string.h>
#include<stdio.h>

struct node {
  int age;
  char *name;
  double score;
};
int a[2]={0};

int main1() {
  int n;
  scanf("%d", &n);
  for (int i=0; i< n; i++)
    a[0]++;
  if (a[0]>5) {
    printf("%d\t%d", a[0], a[1]);
  }
  else 
    printf("%d", a[1]);

}
int main2() {
  node *ptr;
  ptr->age = 18;
  strcpy(ptr->name, "John");
  ptr->score = 99.5;
  printf("%d %s %f.\n", ptr->age, ptr->name, ptr->score);
  node temp;
  temp.score = ptr->score;
  if (temp.score <= 100) {
    printf("%d %s %f.\n", temp.age, temp.name, temp.score);
  }
  else
    printf("nothing.\n");
}
void mem_leak1(int n)
{
  new int;
  int*p=new int;
  if(n!=0)//路径1
  {
    
    int*m=p;
    delete m;
    p=new int;
  }
  else if(n==1)//路径二
  {
    int*q=(int*)malloc(sizeof(int)*n);
  }
  else if(n==2)
  {
    int*u=new int[n];
    delete u;
  }
}
void f0(){
    char str[3][2][2];
    int qq;
    if(qq >0)
      str[2][1][3]='h';
    else
    {
      str[3][0][0]='o';
    }
    str[0][3][3]='p';
    
}
//mixup的分支检测待完善，合并测试的时候可以测出别的就ok
/* int mixup(){
    int * a = NULL;
    int *b = new int;
    int *c =(int *)malloc(sizeof(int));
    int tmp = random();
    if(tmp>0.5){
        if (tmp >0.75)
            delete b;
        else
            a = &tmp;
    }
    else
        free(c);
    *a = 2;
    *b = 3;
    *c = 3;
    return 0;
} */

int malloc(){
    std::string *str = (std::string *)malloc(10);
   // std::cout<<str[0]<<std::endl;
    free(str);
    *str="chick";
    return 0;
}

int New(){
    int *n = new int;
    std::string *c = new std::string;
    int tmp = 0;
    n = &tmp;
    if (tmp >0)
        delete n;
    *n = 2;
    delete c;
    *c ="hahaha";
    return 0;
}

int Fault() {
  int* a;
  a = NULL;
  int b=0;
  *a = 5;
  return 0;
}

int MayFault(){
    double *c;
    double tmp = random();
    if (tmp > 0.3)//该分支不会发生错误
        c = &tmp;   
    else if (tmp <0.2)//该分支会发生错误
        c = NULL;   
    *c = 2.33;    
    return 0;
}

int Contrast(){
    int *b;
    b = NULL;
    int tmp = 1;
    b = &tmp;   //再次define
    *b = 5;     //不发生错误
    return 0;
}

void bof_bcopy(){	
	char argv[20]="01234567890";	
	char sample[10]="a";
	int n=10;
	bcopy(argv, sample, n);
	printf("%s", sample);
}
void bof_sprintf()
{
	char argv[20]="01234567890";
	char buffer[10];
	int num;
	int s=12345;
	num=sprintf(buffer, "%s", argv);
	sprintf(buffer,"%-8d%8d",123,4567);
	sprintf(buffer,"%-8d%8d",s,4567);
	sprintf(buffer,"%d%8d",s,4567);

}
