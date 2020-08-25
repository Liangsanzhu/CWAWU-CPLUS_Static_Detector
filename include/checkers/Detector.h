#ifndef D_T_H
#define D_T_H
#include<iostream>
using namespace std;
#include <fstream>
#include <vector>
#include<map>
#include<queue>
#include<stack>
#include"Def_Use.h"
#include <iomanip>
#include "string.h"
#include "framework/ASTManager.h"

#define CLOSE printf("\033[0m"); //关闭彩色字体
#define LIGHT printf("\033[1m");//高亮
#define RED printf("\033[31m"); //红色字体
#define WHITE printf("\033[37m"); //bai色字体
#define GREEN printf("\033[32m");//绿色字体
#define BLACK printf("\033[30m");//hei色字体
#define YELLOW printf("\033[33m");//黄色字体
#define BLUE printf("\033[34m");//蓝色字体
#define EDGE "\033[36m"

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

int ml=0;
int vud=1;
int bof=2;
int oi=3;
int npd=4;

map<string,vector<string>> SourceCode;

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
  string filter;
  int kind;
 
 bool operator<(const error_info& a) const
    {
	if(kind<a.kind)
	return true;
      if(kind==a.kind&&filename<a.filename)
      return true;
        if(kind==a.kind&&filename==a.filename&&lineno<a.lineno)
        return true;
        if(kind==a.kind&&filename==a.filename&&lineno==a.lineno&&colno<a.colno)
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
//string FILED;uanma 
error_info* new_error_info(error_info*g,string filename,int line,int col,int type,string info,int kind)
{
  error_info*e=new error_info;
  vector<string>t;
 // string a=filename;
  //string s2="/";
  //split(a,s2,&t);
  e->filename=filename;//t[t.size()-1];//文件名
  e->lineno=line;//行
  e->colno=col;//列
  e->type=type;//Error/Note/Warning这种类型
  e->info=info;//Error/Note/Warning:后面跟着的一些文字
  e->next=g;//需要关联报错的下一个指针，需要先创建好
  e->filter="";
  e->kind=kind;
  return e;
 
}

error_info* new_error_info(error_info*g,string filename,int line,int col,int type,string info,string filter,int kind)
{
  error_info*e=new error_info;
  vector<string>t;
 // string a=filename;
  //string s2="/";
  //split(a,s2,&t);
  e->filename=filename;//t[t.size()-1];//文件名
  e->lineno=line;//行
  e->colno=col;//列
  e->type=type;//Error/Note/Warning这种类型
  e->info=info;//Error/Note/Warning:后面跟着的一些文字
  e->next=g;//需要关联报错的下一个指针，需要先创建好
  e->filter=filter;

  e->kind=kind;
  return e;
 
}
void readline()
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
			ML_ERROR_TYPE_MISS=p;
			break;	
		}
		else if(strcmp(q,"ML_ERROR_TYPE_NOTMATCH")==0)
		{
			ML_ERROR_TYPE_NOTMATCH=p;
			break;	
		}
		else if(strcmp(q,"ML_ERROR_TYPE_LOCATION")==0)
		{
			ML_ERROR_TYPE_LOCATION=p;
			break;	
		}
		else if(strcmp(q,"NPD_ERROR_TYPE_DEREFERENCE")==0)
		{
			NPD_ERROR_TYPE_DEREFERENCE=p;	
			break;
		}
		else if(strcmp(q,"NPD_ERROR_TYPE_SET_NULL")==0)
		{
			NPD_ERROR_TYPE_SET_NULL=p;
			break;	
		}
		else if(strcmp(q,"VU_ERROR_TYPE_DEL_A")==0)
		{
			VU_ERROR_TYPE_DEL_A=p;	
			break;
		}
		else if(strcmp(q,"VU_ERROR_TYPE_DEL_B")==0)
		{
			VU_ERROR_TYPE_DEL_B=p;	
			break;
		}
		else if(strcmp(q,"VU_ERROR_TYPE_DEL_A")==0)
		{
			VU_ERROR_TYPE_DEL_A=p;	
			break;
		}
		else if(strcmp(q,"VU_ERROR_TYPE_USE")==0)
		{
			VU_ERROR_TYPE_USE=p;
			break;	
		}
		else if(strcmp(q,"OI_ERROR_TYPE_ARRAY_A")==0)
		{
			OI_ERROR_TYPE_ARRAY_A=p;
			break;	
		}
		else if(strcmp(q,"OI_ERROR_TYPE_ARRAY_B")==0)
		{
			OI_ERROR_TYPE_ARRAY_B=p;
			break;	
		}
		
		}
	    }

	
	}
}


void Get_SourceCode(clang::FunctionDecl*fd)
{

 clang::SourceManager&srcMgr(fd->getASTContext().getSourceManager());
string FILENAME=srcMgr.getFilename(srcMgr.getLocForStartOfFile(srcMgr.getMainFileID()));
       if(SourceCode.find(FILENAME)==SourceCode.end())
      {
        const char*sourcecode=srcMgr.getBuffer(srcMgr.getMainFileID())->getBufferStart();
        int size=srcMgr.getBuffer(srcMgr.getMainFileID())->getBufferSize();
        string p=sourcecode;
        string s2="\n";
        vector<string> SourceCodeItem;
        split(p,s2,&SourceCodeItem);
        SourceCode.insert(pair<string,vector<string>>(FILENAME,SourceCodeItem));
      }
}
int counttemp=0;
int lasttype=-1;

bool print_error(error_info*e, std::ofstream &ofile)
{
 // LIGHT
  //WHITE
 
  if(e==NULL)
  return false;
if(e->filter!=""&&SourceCode.find(e->filename)!=SourceCode.end()&&e->lineno>=1)
 { 
	if(SourceCode.find(e->filename)->second[e->lineno-1].find(e->filter)==string::npos)
		return false;
 }
if(e->type==TYPE_ERROR)
{
LIGHT
BLUE
cout<<counttemp+1<<":";
ofile<<counttemp+1<<":";
CLOSE
}
  LIGHT
  cout<<e->filename<<":"<<e->lineno<<":"<<e->colno<<":";
  ofile<<e->filename<<":"<<e->lineno<<":"<<e->colno<<":";
  switch(e->type)
  {
    case TYPE_ERROR:
    RED
    cout<<" error: ";
    ofile<<" error: ";
    CLOSE
    LIGHT
    break;
    case TYPE_NOTE:
    BLACK
    cout<<" note: ";
    ofile<<" note: ";
    CLOSE
    break;
  }
  //LIGHT
  cout<<e->info<<endl;
  ofile<<e->info<<endl;
  CLOSE
  WHITE
 
   
 if(SourceCode.find(e->filename)!=SourceCode.end()&&e->lineno>=1)
 { 
	cout<<SourceCode.find(e->filename)->second[e->lineno-1]<<endl;
    ofile<<SourceCode.find(e->filename)->second[e->lineno-1]<<endl;
 }
  for(int i=0;i<e->colno-1;i++){
    cout<<" ";
    ofile<<" ";
  }
  LIGHT
  GREEN
  cout<<"^"<<endl;
  ofile<<"^"<<endl;
  CLOSE
  if(e->next!=NULL)
  print_error(e->next,ofile);
return true;
}


void print_result()
{
    //for(auto it:SourceCode)
    //cout<<it<<endl;
    ofstream ofile;
    time_t t = time(0);
    char ch[64];
    strftime(ch, sizeof(ch), "%Y-%m-%d_%H-%M-%S", localtime(&t)); //年-月-日 时-分-秒
    string timestamp = ch;
    string filename = "Error Report/"+timestamp+".txt";
    ofile.open(filename, ios::app);
    if(!ofile)
    {
        cerr<<"Failed to save errors."<<endl;
        return ;
    }
    int count=0;
 while(!result.empty())
  {
    int type_temp=result.top()->kind;
if(type_temp!=lasttype)
{
    if(counttemp>1){
        cout<<counttemp<<" errors generated."<<endl<<endl<<endl;
        ofile<<counttemp<<" errors generated."<<endl<<endl<<endl;
    }
    else if(counttemp==1){
        cout<<counttemp<<" error generated."<<endl<<endl<<endl;
        ofile<<counttemp<<" error generated."<<endl<<endl<<endl;
    }
  counttemp=0;
	LIGHT
	YELLOW
	switch(type_temp){
		case 0:cout<<"[Memory Leak]"<<endl; ofile<<"[Memory Leak]"<<endl;break;
		case 3:cout<<"[Array Out Of Index]"<<endl; ofile<<"[Array Out Of Index]"<<endl;break;
		case 1:cout<<"[Variable Undefined]"<<endl; ofile<<"[Variable Undefined]"<<endl;break;
		case 4:cout<<"[Null Pointer Dereference]"<<endl; ofile<<"[Null Pointer Dereference]"<<endl;break;
		case 2:cout<<"[Buffer Overflow]"<<endl; ofile<<"[Buffer Overflow]"<<endl;break;
	}
	CLOSE
cout<<"---------------------------------------------\n";
ofile<<"---------------------------------------------\n";
	 
}
lasttype=type_temp;


    if(print_error(result.top(),ofile))
	{
		count++;
		counttemp++;
cout<<"---------------------------------------------\n";
ofile<<"---------------------------------------------\n";
	}
    result.pop();
  }
if(counttemp>1){
  cout<<counttemp<<" errors generated."<<endl<<endl<<endl;
  ofile<<counttemp<<" errors generated."<<endl<<endl<<endl;
}
  else if(counttemp==1){
   cout<<counttemp<<" error generated."<<endl<<endl<<endl;
   ofile<<counttemp<<" error generated."<<endl<<endl<<endl;
  }
  counttemp=0;
LIGHT
YELLOW
cout<<"[TOTAL ERROR]"<<endl;
ofile<<"[TOTAL ERROR]"<<endl;
CLOSE
  if(count>1){
  cout<<count<<" errors generated."<<endl;
  ofile<<count<<" errors generated."<<endl;
  }
  else if(count==1){
   cout<<count<<" error generated."<<endl;
   ofile<<count<<" error generated."<<endl;
  }
  else{
    cout<<"no error generated."<<endl;
    ofile<<"no error generated."<<endl;
  }
  ofile.close();
cout<<"Error report has been saved as "+timestamp+".txt"<<endl;
cout<<endl<<endl<<" ██████"<<EDGE<<"╗"<<"\033[0m"<<"██"<<EDGE<<"╗"<<"\033[0m"<<"    ██"<<EDGE<<"╗"<<"\033[0m"<<" █████"<<EDGE<<"╗"<<"\033[0m"<<" ██"<<EDGE<<"╗"<<"\033[0m"<<"    ██"<<EDGE<<"╗"<<"\033[0m"<<"██"<<EDGE<<"╗"<<"\033[0m"<<"   ██"<<EDGE<<"╗"<<"\033[0m"<<"\n\
██"<<EDGE<<"╔"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"    ██"<<EDGE<<"║"<<"\033[0m"<<"██"<<EDGE<<"╔"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<"██"<<EDGE<<"╗"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"    ██"<<EDGE<<"║"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"   ██"<<EDGE<<"║"<<"\033[0m"<<"\n\
██"<<EDGE<<"║"<<"\033[0m"<<"     ██"<<EDGE<<"║"<<"\033[0m"<<" █"<<EDGE<<"╗"<<"\033[0m"<<" ██"<<EDGE<<"║"<<"\033[0m"<<"███████"<<EDGE<<"║"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<" █"<<EDGE<<"╗"<<"\033[0m"<<" ██"<<EDGE<<"║"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"   ██"<<EDGE<<"║"<<"\033[0m"<<"\n\
██"<<EDGE<<"║"<<"\033[0m"<<"     ██"<<EDGE<<"║"<<"\033[0m"<<"███"<<EDGE<<"╗"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"██"<<EDGE<<"╔"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"███"<<EDGE<<"╗"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"   ██"<<EDGE<<"║"<<"\033[0m"<<"\n\
"<<EDGE<<"╚"<<"\033[0m"<<"██████"<<EDGE<<"╗"<<"\033[0m"<<""<<EDGE<<"╚"<<"\033[0m"<<"███"<<EDGE<<"╔"<<"\033[0m"<<"███"<<EDGE<<"╔"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<"██"<<EDGE<<"║"<<"\033[0m"<<"  ██"<<EDGE<<"║"<<"\033[0m"<<""<<EDGE<<"╚"<<"\033[0m"<<"███"<<EDGE<<"╔"<<"\033[0m"<<"███"<<EDGE<<"╔"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<""<<EDGE<<"╚"<<"\033[0m"<<"██████"<<EDGE<<"╔"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<"\n\
 "<<EDGE<<"╚"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<" "<<EDGE<<"╚"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<""<<EDGE<<"╚"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<" "<<EDGE<<"╚"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<"  "<<EDGE<<"╚"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<" "<<EDGE<<"╚"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<""<<EDGE<<"╚"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<"  "<<EDGE<<"╚"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"═"<<"\033[0m"<<""<<EDGE<<"╝"<<"\033[0m"<<""<<endl;

}




#endif
