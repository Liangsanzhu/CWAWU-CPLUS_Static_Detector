#ifndef V_U_D_P_H
#define V_U_D_P_H

#include "def_use_pro.h"
#include "Detector_pro.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"
#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>



class Var_Un_Detector_pro {
public:
  Var_Un_Detector_pro() { ; }

  void detector(defuse_node &all_node, SourceManager *sl, string funcname, std::map<int, Info> global_def) {
    map<int, def_use> node = all_node.get_node();
    map<std::pair<int, int>, def_use> struct_node = all_node.get_struct_node();
    for (auto i = node.begin(); i != node.end(); ++i) {
      var_name = (*i).second.get_vname();
      map<std::pair<int, SourceLocation>, element> du = (*i).second.get_du();
      for (auto ii = du.begin(); ii != du.end(); ++ii) {
        idx = (*ii).first.first;
        def_line = (*ii).second.get_defuse_ln().first;
        def_cl = (*ii).second.get_defuse_cl().first;
        use_line = (*ii).second.get_defuse_ln().second;
        use_cl = (*ii).second.get_defuse_cl().second;
        vector<string> t1;
        vector<string> t2;
        if ((def_line != 0) && (use_line != 0) && ((*ii).second.get_defined() == 0) && (funcname == (*ii).second.get_funcname())) {
          //std::cout << "idx: " << idx << " def: " << def_line << " use: " << use_line << "\n";
          string s2 = "/";
          error_info_pro *e;
          if ((*ii).second.get_defuse_stmt().first != nullptr) {
            string a1 = sl->getFilename((*ii).second.get_defuse_stmt().first->getBeginLoc()).str();
            split_pro(a1, s2, &t1);
            e = new_error_info_pro(NULL, t1[t1.size()-1], use_line, use_cl, TYPE_NOTE, VU_ERROR_TYPE_USE_pro, idx);
          }
          else {
            string a1 = sl->getFilename(global_def[(*i).first].def_decl->getBeginLoc()).str();
            split_pro(a1, s2, &t1);
            e = new_error_info_pro(NULL, t1[t1.size()-1], use_line, use_cl, TYPE_NOTE, VU_ERROR_TYPE_USE_pro, idx);
          }
          if ((*ii).second.get_defuse_stmt().second) {
            string a2 = sl->getFilename((*ii).second.get_defuse_stmt().second->getBeginLoc()).str();
            split_pro(a2, s2, &t2);
            error_info_pro *te = new_error_info_pro(e, t2[t2.size()-1], def_line, def_cl, TYPE_ERROR, VU_ERROR_TYPE_DEL_A_pro+var_name+VU_ERROR_TYPE_DEL_B_pro, idx);
            result_pro.push(te);
          }
        }
        t1.clear();
        t2.clear();
      }
    }
    //struct
    for (auto i = struct_node.begin(); i != struct_node.end(); ++i) {
      var_name = (*i).second.get_vname();
      map<std::pair<int, SourceLocation>, element> du = (*i).second.get_du();
      for (auto ii = du.begin(); ii != du.end(); ++ii) {
        idx = (*ii).first.first;
        def_line = (*ii).second.get_defuse_ln().first;
        def_cl = (*ii).second.get_defuse_cl().first;
        use_line = (*ii).second.get_defuse_ln().second;
        use_cl = (*ii).second.get_defuse_cl().second;
        vector<string> t1;
        vector<string> t2;
        if ((def_line != 0) && (use_line != 0) && ((*ii).second.get_defined() == 0) && (funcname == (*ii).second.get_funcname())) {
          //std::cout << "idx: " << idx << " def: " << def_line << " use: " << use_line << "\n";
          string s2 = "/";
          error_info_pro *e;
          if ((*ii).second.get_defuse_stmt().first != nullptr) {
            string a1 = sl->getFilename((*ii).second.get_defuse_stmt().first->getBeginLoc()).str();
            split_pro(a1, s2, &t1);
            e = new_error_info_pro(NULL, t1[t1.size()-1], use_line, use_cl, TYPE_NOTE, VU_ERROR_TYPE_USE_pro, idx);
          }
          else {
            string a1 = sl->getFilename(global_def[(*i).first.first].def_decl->getBeginLoc()).str();
            split_pro(a1, s2, &t1);
            e = new_error_info_pro(NULL, t1[t1.size()-1], use_line, use_cl, TYPE_NOTE, VU_ERROR_TYPE_USE_pro, idx);
          }
          if ((*ii).second.get_defuse_stmt().second) {
            string a2 = sl->getFilename((*ii).second.get_defuse_stmt().second->getBeginLoc()).str();
            split_pro(a2, s2, &t2);
            error_info_pro *te = new_error_info_pro(e, t2[t2.size()-1], def_line, def_cl, TYPE_ERROR, VU_ERROR_TYPE_DEL_A_pro+var_name+VU_ERROR_TYPE_DEL_B_pro, idx);
            result_pro.push(te);
          }
        }
        t1.clear();
        t2.clear();
      }
    }
    //all_node.output_node(sl);
    
  }

  void VU_Entry(SourceManager&SrcMgr,Stmt*S,int index)
  {
    
  }

private:

  int idx;
  int def_line;
  int use_line;
  int def_cl;
  int use_cl;
  string var_name;
  //string filename;

};

#endif  //V_U_D_H
