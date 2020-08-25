#ifndef D_T_P_H
#define D_T_P_H
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
string error_result_pro="error_result.txt";
string ecout_result_lineno_pro="ecout_lineno.txt";
string source_code_pro="source_code.txt";



#define FLAG_EMPTY -1
#define FLAG_BINARY 1
#define TYPE_ERROR 0
#define TYPE_NOTE 1
string ML_ERROR_TYPE_MISS_pro="Miss memory realese";
string ML_ERROR_TYPE_NOTMATCH_pro="Realese doesn't match allocation";
string ML_ERROR_TYPE_LOCATION_pro="Allocated here";
string NPD_ERROR_TYPE_DEREFERENCE_pro="Find Null Pointer Dereference Fault";
string NPD_ERROR_TYPE_SET_NULL_pro="Set Null or Delete here";
string VU_ERROR_TYPE_DEL_A_pro="Unintialized Varirbale '";
string VU_ERROR_TYPE_DEL_B_pro="' is declared here";
string VU_ERROR_TYPE_USE_pro="Used here";
string OI_ERROR_TYPE_ARRAY_A_pro="Array '";
string OI_ERROR_TYPE_ARRAY_B_pro="' is out of range";
string FILENAME_pro="";
vector<string> SourceCode_pro;

void split_pro(std::string& s,std::string& delim,std::vector<std::string>* ret)
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
struct error_info_pro
{
  int lineno;
  int colno;
  string info;
  string FILENAME_pro;
  int type;
  error_info_pro*next;
  int index;
 bool operator<(const error_info_pro& a) const
    {
       
        if(lineno<a.lineno)
        return true;
        if(lineno==a.lineno&&colno<a.colno)
        return true;
        return false;
    }
   
};
struct cmp_pro //重写仿函数
{
    bool operator() (error_info_pro* a, error_info_pro*b) 
    {
        return !(*a < *b); //大顶堆
    }
};

priority_queue<error_info_pro*,vector<error_info_pro*>,cmp_pro >result_pro;//
map<pair<pair<int,int>,string>,int>result_combine_pro;
priority_queue<error_info_pro*,vector<error_info_pro*>,cmp_pro > result_all_pro;
priority_queue<error_info_pro*,vector<error_info_pro*>,cmp_pro >result_backup_pro;
//string FILED;uanma 
error_info_pro* new_error_info_pro(error_info_pro*g,string FILENAME_pro,int line,int col,int type,string info,int index)
{
  error_info_pro*e=new error_info_pro;
  vector<string>t;
  string a=FILENAME_pro;
  string s2="/";
  split_pro(a,s2,&t);
  e->FILENAME_pro=t[t.size()-1];//文件名
  e->lineno=line;//行
  e->colno=col;//列
  e->type=type;//Error/Note/Warning这种类型
  e->info=info;//Error/Note/Warning:后面跟着的一些文字
  e->next=g;//需要关联报错的下一个指针，需要先创建好
  e->index=index;
  return e;
 
}

 ofstream ecout_pro;
 ofstream sout_pro;

void Get_SourceCode_pro(SourceManager&SMgr)
{
     if(SourceCode_pro.empty())
      {
        FILENAME_pro=SMgr.getFilename(SMgr.getLocForStartOfFile(SMgr.getMainFileID()));
       
        const char*sourcecode=SMgr.getBuffer(SMgr.getMainFileID())->getBufferStart();
        int size=SMgr.getBuffer(SMgr.getMainFileID())->getBufferSize();
        string p=sourcecode;
        string s2="\n";
        split_pro(p,s2,&SourceCode_pro);
       
        
      }
}
void readline_pro()
{

	ifstream infile;
	infile.open("../../error_detail.txt", ios::in);
	if (!infile.is_open())
	{
		cout << "读取缺陷报错配置文件失败" << endl;
		return;
	}
	//第二种读取方法
	char buf[1024];
	while (infile.getline(buf,sizeof(buf)))
	{

	    char *p;
	    const char *delim = ":";
	    p = strtok(buf, delim);
	    while(p) 
	    {
		char*q=p;
		p = strtok(NULL, delim);
		
		if(p)
		{
		
		if(strcmp(q,"ML_ERROR_TYPE_MISS")==0)
		{
			ML_ERROR_TYPE_MISS_pro=p;
			break;	
		}
		else if(strcmp(q,"ML_ERROR_TYPE_NOTMATCH")==0)
		{
			ML_ERROR_TYPE_NOTMATCH_pro=p;
			break;	
		}
		else if(strcmp(q,"ML_ERROR_TYPE_LOCATION")==0)
		{
			ML_ERROR_TYPE_LOCATION_pro=p;
			break;	
		}
		else if(strcmp(q,"NPD_ERROR_TYPE_DEREFERENCE")==0)
		{
			NPD_ERROR_TYPE_DEREFERENCE_pro=p;	
			break;
		}
		else if(strcmp(q,"NPD_ERROR_TYPE_SET_NULL")==0)
		{
			NPD_ERROR_TYPE_SET_NULL_pro=p;
			break;	
		}
		else if(strcmp(q,"VU_ERROR_TYPE_DEL_A")==0)
		{
			VU_ERROR_TYPE_DEL_A_pro=p;	
			break;
		}
		else if(strcmp(q,"VU_ERROR_TYPE_DEL_B")==0)
		{
			VU_ERROR_TYPE_DEL_B_pro=p;	
			break;
		}
		else if(strcmp(q,"VU_ERROR_TYPE_DEL_A")==0)
		{
			VU_ERROR_TYPE_DEL_A_pro=p;	
			break;
		}
		else if(strcmp(q,"VU_ERROR_TYPE_USE")==0)
		{
			VU_ERROR_TYPE_USE_pro=p;
			break;	
		}
		else if(strcmp(q,"OI_ERROR_TYPE_ARRAY_A")==0)
		{
			OI_ERROR_TYPE_ARRAY_A_pro=p;
			break;	
		}
		else if(strcmp(q,"OI_ERROR_TYPE_ARRAY_B")==0)
		{
			OI_ERROR_TYPE_ARRAY_B_pro=p;
			break;	
		}
		
		}
	    }

	
	}
}

int last_pro=-1;
int reportline_pro=0;
void print_error_pro(error_info_pro*e)
{
 // LIGHT
  //WHITE
  if(e==NULL)
  return;
 

 // LIGHT
  ecout_pro<<e->FILENAME_pro<<":"<<e->lineno<<":"<<e->colno<<":";
//reportline++;
  switch(e->type)
  {
    case TYPE_ERROR:
   // RED
    ecout_pro<<" error: ";
   // CLOSE
    //LIGHT
    break;
    case TYPE_NOTE:
    //BLACK
    ecout_pro<<" note: ";
    //CLOSE
    break;
  }
  //LIGHT
  ecout_pro<<e->info<<endl;
reportline_pro++;
 // CLOSE
  //WHITE
  if(e->lineno-1<SourceCode_pro.size()&&e->lineno-1>=0)
  {ecout_pro<<SourceCode_pro[e->lineno-1]<<endl;reportline_pro++;}
  for(int i=0;i<e->colno-1;i++)
    ecout_pro<<" ";
  //LIGHT
  //GREEN
  ecout_pro<<"^"<<endl;
reportline_pro++;
  //CLOSE
  if(e->next!=NULL)
  print_error_pro(e->next);

}

void print_result_pro()
{
    //for(auto it:SourceCode)
   // ecout_pro<<"Detecting……"<<endl;
     ecout_pro.open(error_result_pro);
    int count_pro=0;
result_backup_pro=result_pro;
while(!result_backup_pro.empty())
{
  pair<int,int> pa=pair<int,int>(result_backup_pro.top()->lineno,result_backup_pro.top()->colno);
  pair<pair<int,int>,string> pb= pair<pair<int,int>,string>(pa,result_backup_pro.top()->info);
  if(result_combine_pro.find(pb)==result_combine_pro.end())
  {
    result_combine_pro.insert(pair<pair<pair<int,int>,string>,int>(pb,0));
     result_all_pro.push(result_backup_pro.top());
  }
  result_backup_pro.pop();
}
//ecout_pro<<"[Path ALL]"<<endl;
vector<int> ecout_lineno_pro;
 while(!result_all_pro.empty())
  {
    count_pro++;
ecout_pro<<"--------------------------------------\n";
reportline_pro++;
    print_error_pro(result_all_pro.top());
    ecout_lineno_pro.push_back(reportline_pro);
reportline_pro=0;
    result_all_pro.pop();

}
ecout_pro.close();
ecout_pro.open(ecout_result_lineno_pro);
/*if(count>1)
  ecout_pro<<endl<<count<<" errors generated."<<endl;
  else if(count==1)
   ecout_pro<<endl<<count<<" error generated."<<endl;
   else
   {
     ecout_pro<<endl<<"no error generated."<<endl;
   }*/
   //result_backup=result;
  // ecout_pro<<"****\n";
  for(int i=0;i<ecout_lineno_pro.size();i++)
  {
    ecout_pro<<ecout_lineno_pro[i]<<endl;
  }
  ecout_pro.close();
  
}


#endif
