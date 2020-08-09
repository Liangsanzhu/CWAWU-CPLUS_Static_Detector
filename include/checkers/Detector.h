#ifndef D_T_H
#define D_T_H
#include<iostream>
#include <fstream>
using namespace std;

#include <vector>
#include<map>
#include<queue>
#include<stack>
//#include"Def_Use.h"
#include <iomanip>
#include "string.h"
#include "framework/ASTManager.h"

/*#define CLOSE printf("\033[0m"); //关闭彩色字体
#define LIGHT printf("\033[1m");
#define RED printf("\033[31m"); //红色字体
#define WHITE printf("\033[37m"); //bai色字体
#define GREEN printf("\033[32m");//绿色字体
#define BLACK printf("\033[30m");//hei色字体
#define YELLOW printf("\033[33m");//黄色字体
#define BLUE printf("\033[34m");//蓝色字体*/
string error_result="error_result.txt";
string error_result_lineno="error_lineno.txt";
string source_code="source_code.txt";
string YELLOW="";
string GREEN="";
string RED="";
string GRAY="";
string CLOSE="";


#define FLAG_EMPTY -1
#define FLAG_BINARY 1
#define TYPE_ERROR 0
#define TYPE_NOTE 1
string ML_ERROR_TYPE_MISS="Miss memory realese";
string ML_ERROR_TYPE_NOTMATCH="Realese doesn't match allocation";
string ML_ERROR_TYPE_LOCATION="Allocated here";
string NPD_ERROR_TYPE_DEREFERENCE="Find Null Pointer Dereference Fault";
string NPD_ERROR_TYPE_SET_NULL="Set Null or Delete here";
string VU_ERROR_TYPE_DEL_A="Unintialized Varirbale '";
string VU_ERROR_TYPE_DEL_B="' is declared here";
string VU_ERROR_TYPE_USE="Used here";
string OI_ERROR_TYPE_ARRAY_A="Array '";
string OI_ERROR_TYPE_ARRAY_B="' is out of range";
string FILENAME="";
vector<string> SourceCode;

void split(std::string& s,std::string& delim,std::vector<std::string>* ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim,last);
	while(index != string::npos)
	{
		ret->push_back(s.substr(last,index-last));
		last= index+1;
		index = s.find_first_of(delim,last);
	}
	if(index-last > 0)
	{
		ret->push_back(s.substr(last,index-last));
	}
}
struct error_info
{
  int lineno;
  int colno;
  string info;
  string filename;
  int type;
  error_info*next;
  int index;
 bool operator<(const error_info& a) const
    {
       
        if(lineno<a.lineno)
        return true;
        if(lineno==a.lineno&&colno<a.colno)
        return true;
        return false;
    }
   
};
struct cmp //重写仿函数
{
    bool operator() (error_info* a, error_info*b) 
    {
        return !(*a < *b); //大顶堆
    }
};

priority_queue<error_info*,vector<error_info*>,cmp >result;//
map<pair<pair<int,int>,string>,int>result_combine;
priority_queue<error_info*,vector<error_info*>,cmp > result_all;
priority_queue<error_info*,vector<error_info*>,cmp >result_backup;
//string FILED;uanma 
error_info* new_error_info(error_info*g,string filename,int line,int col,int type,string info,int index)
{
  error_info*e=new error_info;
  vector<string>t;
  string a=filename;
  string s2="/";
  split(a,s2,&t);
  e->filename=t[t.size()-1];//文件名
  e->lineno=line;//行
  e->colno=col;//列
  e->type=type;//Error/Note/Warning这种类型
  e->info=info;//Error/Note/Warning:后面跟着的一些文字
  e->next=g;//需要关联报错的下一个指针，需要先创建好
  e->index=index;
  return e;
 
}

 ofstream eout;
 ofstream sout;

void Get_SourceCode(SourceManager&SMgr)
{
     if(SourceCode.empty())
      {
        FILENAME=SMgr.getFilename(SMgr.getLocForStartOfFile(SMgr.getMainFileID()));
       
        const char*sourcecode=SMgr.getBuffer(SMgr.getMainFileID())->getBufferStart();
        int size=SMgr.getBuffer(SMgr.getMainFileID())->getBufferSize();
        string p=sourcecode;
        string s2="\n";
        split(p,s2,&SourceCode);
       
        
      }
}
int last=-1;

void print_error(error_info*e)
{
 // LIGHT
  //WHITE
  if(e==NULL)
  return;
 

 // LIGHT
  eout<<e->filename<<":"<<e->lineno<<":"<<e->colno<<":";
  switch(e->type)
  {
    case TYPE_ERROR:
   // RED
    eout<<RED<<" error: "<<CLOSE;
   // CLOSE
    //LIGHT
    break;
    case TYPE_NOTE:
    //BLACK
    eout<<GRAY<<" note: "<<CLOSE;
    //CLOSE
    break;
  }
  //LIGHT
  eout<<e->info<<endl;
 // CLOSE
  //WHITE
  if(e->lineno-1<SourceCode.size()&&e->lineno-1>=0)
  eout<<SourceCode[e->lineno-1]<<endl;
  for(int i=0;i<e->colno-1;i++)
    eout<<" ";
  //LIGHT
  //GREEN
  eout<<GREEN<<"^"<<CLOSE<<endl;
  //CLOSE
  if(e->next!=NULL)
  print_error(e->next);

}

void print_result()
{
    //for(auto it:SourceCode)
   // eout<<"Detecting……"<<endl;
     eout.open(error_result);
    int count=0;
result_backup=result;
while(!result_backup.empty())
{
  pair<int,int> pa=pair<int,int>(result_backup.top()->lineno,result_backup.top()->colno);
  pair<pair<int,int>,string> pb= pair<pair<int,int>,string>(pa,result_backup.top()->info);
  if(result_combine.find(pb)==result_combine.end())
  {
    result_combine.insert(pair<pair<pair<int,int>,string>,int>(pb,0));
     result_all.push(result_backup.top());
  }
  result_backup.pop();
}
eout<<"[Path ALL]"<<endl;
vector<int> error_lineno;
 while(!result_all.empty())
  {
    count++;
eout<<"--------------------------------------\n";
    print_error(result_all.top());
    error_lineno.push_back(result_all.top()->lineno);
    result_all.pop();

}
eout.close();
eout.open(error_result_lineno);
if(count>1)
  eout<<endl<<count<<" errors generated."<<endl;
  else if(count==1)
   eout<<endl<<count<<" error generated."<<endl;
   else
   {
     eout<<endl<<"no error generated."<<endl;
   }
   //result_backup=result;
  // eout<<"****\n";
  for(int i=0;i<error_lineno.size();i++)
  {
    eout<<error_lineno[i]<<endl;
  }
  eout.close();
  
}


#endif