#ifndef B_O_F_H
#define B_O_F_H
#include "Detector.h"

class Buffer_Overflow
{
private:
    //void handleDeclStmt(const Stmt *S);                     //处理DeclStmt
    //void handleCallExpr(const Stmt *S);                     //处理CallExpr
    //int strLen(string type);                                //得到数组变量char[]的长度
    //void detect_bcopy(const Stmt *S, FunctionDecl *callee); //检测bcopy()函数

    map<std::string, int> Var;
    map<int, int> Format_param;
    map<int, SourceLocation> FuncLocation;
    int ifindex;
    defuse_node *all_node;

    struct bof_error
    {
        string filename;
        int line;
        int col;
        string info;
    };
    map<int, bof_error> bof_info;

    void handleDeclStmt(const Stmt *S)
    {
        DeclStmt *declstmt = (DeclStmt *)S;

        auto it = declstmt->decl_begin();
        VarDecl *vdecl = static_cast<VarDecl *>(*it);
        string var_tpye = vdecl->getType().getAsString();
        string var_name = vdecl->getIdentifier()->getName().data();
        int len = strLen(var_tpye);

        //cout << "var tpye:   " << var_tpye << endl;
        //cout << "var name:   " << var_name << endl;
        //cout << "char len= " << len << endl;
        //cout << endl;

        Var.insert(pair<string, int>(var_name, len));

        for (auto i = S->child_begin(); i != S->child_end(); i++)
        {
            //std::cout << "StmtClass:  " << i->getStmtClassName() << std::endl;
            if (strcmp(i->getStmtClassName(), "IntegerLiteral") == 0)
            {
                IntegerLiteral *intltr = (IntegerLiteral *)*i;
                int int_val = intltr->getValue().getSExtValue();
                //cout << "var value: " << int_val << endl;
                Var[var_name] = int_val;
                //cout << var_name << "=" << Var[var_name] << endl;
            }

            if (strcmp(i->getStmtClassName(), "StringLiteral") == 0)
            {
                StringLiteral *strltr = (StringLiteral *)*i;
                //cout << "string data: " << strltr->getString().data() << endl;
            }
            //cout << endl;
        }
    }

    void handleCallExpr(const Stmt *S)
    {
        CallExpr *callexpr = (CallExpr *)S;
        FunctionDecl *callee = callexpr->getDirectCallee();
        string callee_name = callee->getNameAsString();

        if (callee_name == "bcopy" || callee_name == "memcpy" || callee_name == "memset" ||  callee_name == "strncpy")
        {
            int callee_num = FuncLocation.size();
            FuncLocation.insert(pair<int, SourceLocation>(callee_num, S->getBeginLoc()));
            detect_bcopy(S, callee, callee_num); //检测bcopy()函数
        }
        if (callee_name == "sprintf")
        {
            int callee_num = FuncLocation.size();
            FuncLocation.insert(pair<int, SourceLocation>(callee_num, S->getBeginLoc()));
            detect_sprintf(S, callee, callee_num);
        }
        if (callee_name == "scanf")
        {
            int callee_num = FuncLocation.size();
            FuncLocation.insert(pair<int, SourceLocation>(callee_num, S->getBeginLoc()));
            detect_scanf(S, callee, callee_num);
        }
        if (callee_name == "snprintf")
        {
            int callee_num = FuncLocation.size();
            FuncLocation.insert(pair<int, SourceLocation>(callee_num, S->getBeginLoc()));
            detect_snprintf(S, callee, callee_num);
        }
    }

    void detect_bcopy(const Stmt *S, FunctionDecl *callee, int callee_num)
    {
        int len_dest = -1;
        int len_src = -1;
        int len_n = -1;

        int param_order[3]; //0-src,1-dest,2-n
        for (int i = 0; i < 3; i++)
            param_order[i] = -1;

        auto param = callee->param_begin();
        int num = 1;

        string Func_name = callee->getNameAsString();

        for (; param != callee->param_end(); param++)
        {
            string param_name = (*param)->getIdentifier()->getName().data();

            //cout << "!!!----------------param name:   " << param_name << endl;
            //std::cout << "!!!-------------param type:   " << (*param)->getType().getAsString() << std::endl;
            if (param_name == "__src" || param_name == "__s")
                param_order[0] = num;
            else if (param_name == "__dest" || param_name == "__c")
                param_order[1] = num;
            else if (param_name == "__n")
                param_order[2] = num;
            num++;
        }

        //cout << param_order[0] << " " << param_order[1] << " " << param_order[2] << endl;
        //cout << endl;

        num = 0;
        auto i = S->child_begin();
        for (auto i = S->child_begin(); i != S->child_end(); i++)
        {
            auto j = i;
            if (Func_name != "memset" || num != 2)
            {
                while (strcmp(j->getStmtClassName(), "DeclRefExpr") != 0)
                {
                    j = j->child_begin();
                    //cout << "j: " << j->getStmtClassName() << endl;
                }
                DeclRefExpr *declre = (DeclRefExpr *)*j;
                string param_name = declre->getNameInfo().getAsString();
                string param_type = declre->getType().getAsString();

                //cout << "param name:  " << param_name << endl;
                //cout << "param type:  " << param_type << endl;
                //cout << "num=" << num << endl;

                if (num == param_order[1])
                {
                    len_dest = Var[param_name];
                    //cout << "len_dest=" << len_dest << endl;
                }
                else if (num == param_order[2])
                {
                    len_n = Var[param_name];
                    //cout << "len_n=" << len_n << endl;
                }
                else if (num == param_order[0])
                {
                    len_src = Var[param_name];
                }
            }
            num++;
            //cout << endl;
        }

        if (param_order[2] == -1)
        {
            len_n = len_src;
        }

        //
        if (len_n == -1)
        {
            //warning:maybe right or wrong
        }
        else
        {
            if (len_dest <= len_n)
            {
                clang::SourceManager &srcmgr = callee->getASTContext().getSourceManager();
                //error
                //cout << "error:" << endl;
                string Func_name = callee->getNameAsString();

                bof_error tmp_info;
                tmp_info.filename = srcmgr.getFilename(FuncLocation[callee_num]).str();
                tmp_info.line = srcmgr.getSpellingLineNumber(FuncLocation[callee_num]);
                tmp_info.col = srcmgr.getSpellingColumnNumber(FuncLocation[callee_num]);
                tmp_info.info = "buffer overflow caused by use " + Func_name + " to copy data";

                int tmp_num = bof_info.size();
                bof_info.insert(pair<int, bof_error>(tmp_num, tmp_info));

                //error_info *e = new_error_info(NULL, filename, line, col, TYPE_ERROR, info, ifindex);
                //result.push(e);
            }
            else
            {
                //no error or warning
            }
        }
    }

    void detect_sprintf(const Stmt *S, FunctionDecl *callee, int callee_num)
    {
        int len_dest = -1;
        int len_src = -1;
        int len_n = -1;
        string format_str;
        int num = 0;
        auto i = S->child_begin();
        while (i != S->child_end())
        {
            auto j = i;
            while (true)
            {
                //cout << "j: " << j->getStmtClassName() << endl;
                if (strcmp(j->getStmtClassName(), "DeclRefExpr") == 0)
                {
                    DeclRefExpr *declre = (DeclRefExpr *)*j;
                    string param_name = declre->getNameInfo().getAsString();
                    string param_type = declre->getType().getAsString();
                    if (num >= 3)
                    {
                        int temp = Var[param_name];
                        if (temp == 0)
                            temp = -1;
                        Format_param.insert(pair<int, int>(num - 3, temp));
                        Format_param[num - 3] = temp;
                    }
                    if (num == 1)
                    {
                        len_dest = Var[param_name];
                        //cout << "len_dest:  " << len_dest << endl;
                    }
                    //cout << "param name:  " << param_name << endl;
                    //cout << "param type:  " << param_type << endl;
                    break;
                }
                else if (strcmp(j->getStmtClassName(), "StringLiteral") == 0)
                {
                    StringLiteral *strltr = (StringLiteral *)*j;
                    if (num <= 2)
                    {
                        format_str = strltr->getString().str();
                        //cout << "str: " << format_str << endl;
                    }
                    else
                    {
                        string temp_str = strltr->getString().str();
                        Format_param.insert(pair<int, int>(num - 3, temp_str.size()));
                        Format_param[num - 3] = temp_str.size();
                        //cout << "str: " << temp_str << endl;
                    }
                    break;
                }
                else if (strcmp(j->getStmtClassName(), "IntegerLiteral") == 0)
                {
                    IntegerLiteral *intltr = (IntegerLiteral *)*j;
                    int temp_int = intltr->getValue().getSExtValue();
                    Format_param.insert(pair<int, int>(num - 3, temp_int));
                    Format_param[num - 3] = temp_int;
                    //cout << "int: " << temp_int << endl;
                    break;
                }
                else if (j->child_begin() == nullptr)
                {
                    //cout << "none child" << endl;
                    break;
                }
                else
                    j = j->child_begin();
            }
            //cout << "num=" << num << endl;

            i++;
            num++;
            //cout << endl;
        }

        int dLen;
        int totalLen = count_totalLen(format_str, dLen);

        //cout << "len_dest" << len_dest << endl;
        //cout << "totalLen" << totalLen << endl;
        if (len_dest <= totalLen)
        {
            clang::SourceManager &srcmgr = callee->getASTContext().getSourceManager();
            //error
            //cout << "\033[32m error:there is a wrong \033[0m" << endl;
            //cout << "error:" << endl;
            string Func_name = callee->getNameAsString();

            bof_error tmp_info;
            tmp_info.filename = srcmgr.getFilename(FuncLocation[callee_num]).str();
            tmp_info.line = srcmgr.getSpellingLineNumber(FuncLocation[callee_num]);
            tmp_info.col = srcmgr.getSpellingColumnNumber(FuncLocation[callee_num]);
            tmp_info.info = "buffer overflow caused by use " + Func_name + " to format data";

            int tmp_num = bof_info.size();
            bof_info.insert(pair<int, bof_error>(tmp_num, tmp_info));
        }
        else if (dLen == -1)
        {
            //warning:maybe right or wrong
            //cout << "\033[32m warning:maybe right or wrong \033[0m" << endl;
        }
        else
        {
            //no error or warning
            //cout << "\033[32m no error or warning \033[0m" << endl;
        }
    }

    void detect_scanf(const Stmt *S, FunctionDecl *callee, int callee_num)
    {
        map<int, int> len_dest;
        map<int, int> len_n;
        string format_str;
        int num = 0;
        auto i = S->child_begin();
        while (i != S->child_end())
        {
            auto j = i;

            while (true)
            {
                //cout << "j: " << j->getStmtClassName() << endl;
                if (strcmp(j->getStmtClassName(), "DeclRefExpr") == 0)
                {
                    DeclRefExpr *declre = (DeclRefExpr *)*j;
                    string param_name = declre->getNameInfo().getAsString();
                    string param_type = declre->getType().getAsString();
                    if (num >= 2)
                    {
                        int temp = Var[param_name];
                        if (temp == 0)
                            temp = -1;
                        len_dest.insert(pair<int, int>(num - 2, temp));
                        len_dest[num - 2] = temp;
                    }
                    break;
                }
                else if (strcmp(j->getStmtClassName(), "StringLiteral") == 0)
                {
                    StringLiteral *strltr = (StringLiteral *)*j;
                    if (num <= 1)
                    {
                        format_str = strltr->getString().str();
                        //cout << "str: " << format_str << endl;
                    }
                    break;
                }
                else if (j->child_begin() == nullptr)
                {
                    //cout << "none child" << endl;
                    break;
                }
                else
                    j = j->child_begin();
            }
            //cout << "num=" << num << endl;

            i++;
            num++;
            //cout << endl;
        }

        int cur = 0;
        for (int i = 0; i < format_str.size(); i++)
        {
            if (format_str[i] == '%')
            {
                i++;
                int temp = 0;
                while (format_str[i] <= '9' && format_str[i] >= '0')
                {
                    temp = temp * 10 + format_str[i] - '0';
                    i++;
                }
                if (format_str[i] == 's')
                {
                    len_n.insert(pair<int, int>(cur, temp));
                    len_n[cur] = temp;
                    cur++;
                }
                else
                {
                    temp = -2;
                    len_n.insert(pair<int, int>(cur, temp));
                    len_n[cur] = temp;
                    cur++;
                }
            }
        }

        bool buffover_error = false;
        bool buffover_warnning = false;

        for (int i = 0; i < cur; i++)
        {
           // std::cout << "\033[32m" << i << "----"
             //         << "len_n:" << len_n[i] << "   len_dest" << len_dest[i] << "\033[0m" << std::endl;
            if (len_n[i] != -2)
            {
                if (len_n[i] >= len_dest[i])
                {
                    buffover_error = true;
                }
            }
        }

        if (buffover_error)
        {
            clang::SourceManager &srcmgr = callee->getASTContext().getSourceManager();
            //error
            //cout << "\033[32m error:there is a wrong \033[0m" << endl;
            //cout << "error:" << endl;
            string Func_name = callee->getNameAsString();

            bof_error tmp_info;
            tmp_info.filename = srcmgr.getFilename(FuncLocation[callee_num]).str();
            tmp_info.line = srcmgr.getSpellingLineNumber(FuncLocation[callee_num]);
            tmp_info.col = srcmgr.getSpellingColumnNumber(FuncLocation[callee_num]);
            tmp_info.info = "buffer overflow caused by use " + Func_name + " to format data";

            int tmp_num = bof_info.size();
            bof_info.insert(pair<int, bof_error>(tmp_num, tmp_info));
        }
        else if (buffover_warnning)
        {
            //warning:maybe right or wrong
            //cout << "\033[32m warning:maybe right or wrong \033[0m" << endl;
        }
        else
        {
            //no error or warning
            //cout << "\033[32m no error or warning \033[0m" << endl;
        }
    }

    void detect_snprintf(const Stmt *S, FunctionDecl *callee, int callee_num)
    {
        int len_dest = -1;
        int len_src = -1;
        int len_n = -1;
        string format_str;
        int num = 0;
        auto i = S->child_begin();
        while (i != S->child_end())
        {
            auto j = i;
            while (true)
            {
                //cout << "j: " << j->getStmtClassName() << endl;
                if (strcmp(j->getStmtClassName(), "DeclRefExpr") == 0)
                {
                    DeclRefExpr *declre = (DeclRefExpr *)*j;
                    string param_name = declre->getNameInfo().getAsString();
                    string param_type = declre->getType().getAsString();
                    if (num >= 4)
                    {
                        int temp = Var[param_name];
                        if (temp == 0)
                            temp = -1;
                        Format_param.insert(pair<int, int>(num - 4, temp));
                        Format_param[num - 4] = temp;
                    }
                    if (num == 2)
                    {
                        len_n = Var[param_name];
                    }
                    if (num == 1)
                    {
                        len_dest = Var[param_name];
                        //cout << "len_dest:  " << len_dest << endl;
                    }
                    //cout << "param name:  " << param_name << endl;
                    //cout << "param type:  " << param_type << endl;
                    break;
                }
                else if (strcmp(j->getStmtClassName(), "StringLiteral") == 0)
                {
                    StringLiteral *strltr = (StringLiteral *)*j;
                    if (num <= 3)
                    {
                        format_str = strltr->getString().str();
                        //cout << "str: " << format_str << endl;
                    }
                    else
                    {
                        string temp_str = strltr->getString().str();
                        Format_param.insert(pair<int, int>(num - 4, temp_str.size()));
                        Format_param[num - 4] = temp_str.size();
                        //cout << "str: " << temp_str << endl;
                    }
                    break;
                }
                else if (strcmp(j->getStmtClassName(), "IntegerLiteral") == 0)
                {

                    IntegerLiteral *intltr = (IntegerLiteral *)*j;
                    int temp_int = intltr->getValue().getSExtValue();
                    if (num >= 4)
                    {
                        Format_param.insert(pair<int, int>(num - 4, temp_int));
                        Format_param[num - 4] = temp_int;
                        //cout << "int: " << temp_int << endl;
                    }
                    else if (num == 2)
                    {
                        len_n = temp_int;
                    }

                    break;
                }
                else if (j->child_begin() == nullptr)
                {
                    //cout << "none child" << endl;
                    break;
                }
                else
                    j = j->child_begin();
            }
            //cout << "num=" << num << endl;

            i++;
            num++;
            //cout << endl;
        }

        int dLen;
        int totalLen = count_totalLen(format_str, dLen);
        //std::cout << "\033[32m" << len_dest<<"   "<<len_n<<"    "<<totalLen<< "\033[0m" << std::endl;
        //cout << "len_dest" << len_dest << endl;
        //cout << "totalLen" << totalLen << endl;
        //if (len_dest <= totalLen)
        if (totalLen >= len_dest && len_n > len_dest)
        {
            clang::SourceManager &srcmgr = callee->getASTContext().getSourceManager();
            //error
            //cout << "\033[32m error:there is a wrong \033[0m" << endl;
            //cout << "error:" << endl;
            string Func_name = callee->getNameAsString();

            bof_error tmp_info;
            tmp_info.filename = srcmgr.getFilename(FuncLocation[callee_num]).str();
            tmp_info.line = srcmgr.getSpellingLineNumber(FuncLocation[callee_num]);
            tmp_info.col = srcmgr.getSpellingColumnNumber(FuncLocation[callee_num]);
            tmp_info.info = "buffer overflow caused by use " + Func_name + " to format data";

            int tmp_num = bof_info.size();
            bof_info.insert(pair<int, bof_error>(tmp_num, tmp_info));
        }
        else if (dLen == -1)
        {
            //warning:maybe right or wrong
            //cout << "\033[32m warning:maybe right or wrong \033[0m" << endl;
        }
        else
        {
            //no error or warning
            //cout << "\033[32m no error or warning \033[0m" << endl;
        }
    }

    int strLen(string type)
    {
        string t;
        int len = 0;
        for (int i = 0; i < 6; i++)
            t = t + type[i];

        if (t == "char [")
            for (int i = 6; type[i] != ']'; i++)
                len = len * 10 + type[i] - '0';
        else
            len = -1;
        return len;
    }

    int count_totalLen(string format_str, int &dLen)
    {
        int totalLen = 0;
        dLen = 0;
        //cout << "str size:  " << format_str.size() << endl;
        int num_param = 0;
        for (int cur = 0; cur < format_str.size(); cur++)
        {
            //cout << "str[" << cur << "]: "
            //<< " " << format_str[cur] << endl;
            if (format_str[cur] == '%')
            {
                if (format_str[cur + 1] == '-')
                {
                    cur++;
                }
                int len_1 = -1;
                int len_2 = -1;
                int len_str = 0;
                cur++;
                char fmt_type = detect_formattingStr(format_str, cur, len_1, len_2);
                //cout << "len_1: " << len_1 << "   "
                // << "len_2:  " << len_2 << endl;
                //cout << "fmt_type: " << fmt_type << endl;
                if (len_1 != -1)
                {
                    totalLen += len_1;
                    if (len_2 != -1 && dLen != -1)
                        dLen += len_2;
                }
                else
                {
                    int temp = Format_param[num_param];
                    if (fmt_type == 's')
                    {
                        if (temp != -1)
                        {
                            totalLen += temp;
                        }
                        else
                        {
                            dLen = -1;
                        }
                    }
                    else if (fmt_type == 'd')
                    {
                        int temp_len = 0;
                        while (temp != 0)
                        {
                            //cout << "temp=  " << temp << endl;
                            temp_len++;
                            temp = temp / 10;
                        }
                        totalLen += temp_len;
                    }
                }
                num_param++;
            }
            else
            {
                totalLen++;
            }
            //cout << endl;
        }

        //cout << "totlaLen:  " << totalLen << endl;
        //cout << "dLen:  " << dLen << endl;
        return totalLen;
    }

    char detect_formattingStr(string str, int &cur, int &len_1, int &len_2)
    {
        /* auto i = S->child_begin();
  for (int temp = 0; temp < num; temp++)
  {
    i++;
  }*/
        //cout << "////start////" << endl;
        char format_type;
        len_1 = -1;
        len_2 = -1;
        while (cur < str.size())
        {
            if (str[cur] >= '0' && str[cur] < '9')
            {
                if (len_2 == -1)
                {
                    if (len_1 == -1)
                        len_1 = 0;
                    len_1 = len_1 * 10 + str[cur] - '0';
                    //cout << "len_1: " << len_1 << endl;
                }
                else
                {
                    len_2 = len_2 * 10 + str[cur] - '0';
                    //cout << "len_2: " << len_1 << endl;
                }
            }
            else if (str[cur] == '.')
            {
                len_2 == 0;
            }
            else
            {
                format_type = str[cur];
                break;
            }
            cur++;
        }
        return format_type;
    }

public:
    void BOF_Entry_old(clang::FunctionDecl *fd)
    {

        //std::cout << common::getFullName(fd) << std::endl;
        auto fd_cfg = common::buildCFG(fd);
        // Traverse CFG

        //////////////////////////////////////////////////////
        //std::cout << "- - - - - - Start - - - - - -" << std::endl;
        auto it_block = fd_cfg->begin();
        for (; it_block != fd_cfg->end(); it_block++)
        {
            auto it_element = (*it_block)->begin();
            for (; it_element != (*it_block)->end(); it_element++)
            {
                Optional<CFGStmt> it_stmt = it_element->getAs<CFGStmt>();
                const Stmt *S = it_stmt->getStmt();
                char const *StmtClassName = S->getStmtClassName();
                //std::cout << "\033[32m" << StmtClassName << "\033[0m" << std::endl;

                int isDeclStmt = strcmp(StmtClassName, "DeclStmt");
                int isCallExpr = strcmp(StmtClassName, "CallExpr");

                if (isDeclStmt == 0)
                    handleDeclStmt(S); //处理DeclStmt

                if (isCallExpr == 0)
                    handleCallExpr(S); //处理CallExpr
            }
        }
        //std::cout << "- - - - - - End - - - - - -" << std::endl;
    }

    void BOF_Entry(SourceManager &SrcMgr, Stmt *S, int index, defuse_node *ano)
    {
        all_node = ano;
        ifindex = index;
        char const *StmtClassName = S->getStmtClassName();
        //std::cout << "\033[32m" << StmtClassName << "\033[0m" << std::endl;

        int isDeclStmt = strcmp(StmtClassName, "DeclStmt");
        int isCallExpr = strcmp(StmtClassName, "CallExpr");
        if (isDeclStmt == 0)
            handleDeclStmt(S); //处理DeclStmt

        if (isCallExpr == 0)
            handleCallExpr(S); //处理CallExpr
    }

    void BOF_Detect()
    {
        for (int i = 0; i < bof_info.size(); i++)
        {
            bof_error tmp = bof_info[i];
            error_info *e = new_error_info(NULL, tmp.filename, tmp.line, tmp.col, TYPE_ERROR, tmp.info, ifindex);
            result.push(e);
        }
    }
};
#endif
