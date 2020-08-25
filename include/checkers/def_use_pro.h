#ifndef DEF_USE_P_H
#define DEF_USE_P_H

#include <string>
#include <string.h>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <utility>

class element {
public:
  
  element() {
    defuse_stmt.first = (Stmt *)malloc(sizeof(Stmt));
    defuse_stmt.second = (Stmt *)malloc(sizeof(Stmt));
    defined = 0;
    block_id = -1;
  }

  void add_defuse_ln(int def, int use) {
    std::pair<int, int> pr(def, use);
    defuse_ln = pr;
  }

  std::pair<int, int> get_defuse_ln() {
    return defuse_ln;
  }

  void add_defuse_cl(int def, int use) {
   std::pair<int, int> pr(def, use);
   defuse_cl = pr;
  }

  std::pair<int, int> get_defuse_cl() {
    return defuse_cl;
  }

  void add_funcname(std::string funcname) {
    func_name = funcname;
  }

  std::string get_funcname() {
    return func_name;
  }

  void add_defined(int d) {
    defined = d;
  }

  int get_defined() {
    return defined;
  }

  void add_blockid(int id) {
    block_id = id;
  }

  int get_blockid() {
    return block_id;
  }

  void add_defuse_stmt(Stmt *defstmt, Stmt *usestmt) {
    defuse_stmt.first = defstmt;
    defuse_stmt.second = usestmt;
  }

  std::pair<Stmt *, Stmt *> get_defuse_stmt() {
    return defuse_stmt;
  }

  void output_element() {
  /*  std::cout << defuse_ln.first << " " << defuse_ln.second << "\n";
    std::cout << defuse_cl.first << " " << defuse_cl.second << "\n";
    std::cout << "Belong to Function: " << func_name << "\n";
    if (defuse_stmt.first != nullptr)
      std::cout << defuse_stmt.first->getStmtClassName() << "\n";
    if (defuse_stmt.second != nullptr)
      std::cout << defuse_stmt.second->getStmtClassName() << "\n";
    std::cout << block_id << "\n";
    std::cout << "defined: " << defined << "\n";*/
  }
 

private:
  std::pair<int, int> defuse_ln;
  std::pair<int, int> defuse_cl;
  std::string func_name;
  //when defuse_ln == 0
  std::pair<Stmt *, Stmt *> defuse_stmt;
  int defined;
  int block_id;

};

class def_use {
public:

  def_use() {
    ;
  }

  void add_defuse_ln(int idx, SourceLocation sl, int def_ln, int use_ln, int def_cl, int use_cl) {
    std::pair<int, SourceLocation> pr(idx, sl);
    du[pr].add_defuse_ln(def_ln, use_ln);
    du[pr].add_defuse_cl(def_cl, use_cl);
  }

  void add_defuse_stmt(int idx, SourceLocation sl, Stmt *def_stmt, Stmt *use_stmt) {
    std::pair<int, SourceLocation> pr(idx, sl);
    du[pr].add_defuse_stmt(def_stmt, use_stmt);
  }

  void add_funcname(int idx, SourceLocation sl, std::string name) {
    std::pair<int, SourceLocation> pr(idx, sl);
    du[pr].add_funcname(name);
  }

  void add_blockid(int idx, SourceLocation sl, int id) {
    std::pair<int, SourceLocation> pr(idx, sl);
    du[pr].add_blockid(id);
  }

  void add_defined(int idx, SourceLocation sl, int defined) {
    std::pair<int, SourceLocation> pr(idx, sl);
    du[pr].add_defined(defined);
  }

  /*
  void add_Info(int def, int use, std::string funcname, Stmt *defstmt, Stmt *usestmt) {
    std::pair<int, int> pr(def, use);
    if (du.size() > 0) {
      auto i = du.begin();
      for (; i != du.end(); ++i) {
        if (pr == (*i).get_lncol()) {
          break;
        }
      }
      if (pr != (*i).get_lncol()) {
        element p;
        p.add_lncol(def, use);
        p.add_funcname(funcname);
        p.add_defuse_stmt(defstmt, usestmt);
        du.push_back(p);
      }
    }
    else {
      element p;
      p.add_funcname(funcname);
      p.add_lncol(def, use);
      p.add_defuse_stmt(defstmt, usestmt);
      du.push_back(p);
    }
  }
  */

  void add_varname(std::string v) {
    var_name = v;
  }

  void add_type(std::string t) {
    type = t;
  }

  std::string get_type() {
    return type;
  }

  std::string get_vname() {
    return var_name;
  }

  element get_element(int idx, SourceLocation sl) {
    std::pair<int, SourceLocation> pr(idx, sl);
    return du[pr];
  }

  std::map<std::pair<int, SourceLocation>, element> get_du() {
    return du;
  }

  void output_all(SourceManager *scm) {
   /* std::cout << var_name << "  ";
    std::cout << type << "\n";
    for (auto i = du.begin(); i != du.end(); ++i) {
      std::cout << (*i).first.first << "\n";
      std::cout << (*i).first.second.printToString(*scm);
      std::cout << "\n";
      (*i).second.output_element();
    }*/
  }

private:
  //maybe modified to map type
  std::map<std::pair<int, SourceLocation>, element> du;
  //type of the variable
  std::string type;
  std::string var_name;
  
};

class defuse_node {
public:

  defuse_node() {
    ;
  }

  //general data
  void add_var(int id, std::string var, std::string type) {
    du_node[id] = def_use();
    du_node[id].add_varname(var);
    du_node[id].add_type(type);
  }

  void add_defuse_ln(int id, int idx, SourceLocation sl, int def, int use, int def_cl, int use_cl) {
    du_node[id].add_defuse_ln(idx, sl, def, use, def_cl, use_cl);
  }

  void add_funcname(int id, int idx, SourceLocation sl, std::string name) {
    du_node[id].add_funcname(idx, sl, name);
  }

  void add_defuse_stmt(int id, int idx, SourceLocation sl, Stmt *def_stmt, Stmt *use_stmt) {
    du_node[id].add_defuse_stmt(idx, sl, def_stmt, use_stmt);
  }

  void add_blockid(int id, int idx, SourceLocation sl, int blockid) {
    du_node[id].add_blockid(idx, sl, blockid);
  }

  void add_defined(int id, int idx, SourceLocation sl, int defined) {
    du_node[id].add_defined(idx, sl, defined);
  }

  void output_node(SourceManager *scm) {
    for (auto b = du_node.begin(); b != du_node.end(); ++b) {
      std::cout << (*b).first << "  ";
      (*b).second.output_all(scm);
    }
  }

  def_use get_defuse(int id) {
    return du_node[id];
  }

  element get_element(int id, int idx, SourceLocation sl) {
    du_node[id].get_element(idx, sl);
  }

  std::string get_type(int id) {
    return du_node[id].get_type();
  }

  std::string get_varname(int id) {
    return du_node[id].get_vname();
  }

  std::map<int, def_use> get_node() {
    return du_node;
  }

  //struct data
  void add_struct_var(std::pair<int, int> id, std::string var, std::string type) {
    struct_node[id] = def_use();
    struct_node[id].add_varname(var);
    struct_node[id].add_type(type);
  }

  void add_struct_defuse_ln(std::pair<int, int> id, int idx, SourceLocation sl, int def, int use, int def_cl, int use_cl) {
    struct_node[id].add_defuse_ln(idx, sl, def, use, def_cl, use_cl);
  }

  void add_struct_funcname(std::pair<int, int> id, int idx, SourceLocation sl, std::string name) {
    struct_node[id].add_funcname(idx, sl, name);
  }

  void add_struct_defuse_stmt(std::pair<int, int> id, int idx, SourceLocation sl, Stmt *def_stmt, Stmt *use_stmt) {
    struct_node[id].add_defuse_stmt(idx, sl, def_stmt, use_stmt);
  }

  void add_struct_blockid(std::pair<int, int> id, int idx, SourceLocation sl, int blockid) {
    struct_node[id].add_blockid(idx, sl, blockid);
  }

  void add_struct_defined(std::pair<int, int> id, int idx, SourceLocation sl, int defined) {
    struct_node[id].add_defined(idx, sl, defined);
  }

  void output_struct_node(SourceManager *scm) {
    /*for (auto b = struct_node.begin(); b != struct_node.end(); ++b) {
      std::cout << (*b).first.first << "  ";
      std::cout << (*b).first.second << " ";
      (*b).second.output_all(scm);
    }*/
  }

  def_use get_struct_defuse(std::pair<int, int> id) {
    return struct_node[id];
  }

  element get_struct_element(std::pair<int, int> id, int idx, SourceLocation sl) {
    struct_node[id].get_element(idx, sl);
  }

  std::string get_struct_type(std::pair<int, int> id) {
    return struct_node[id].get_type();
  }

  std::string get_struct_varname(std::pair<int, int> id) {
    return struct_node[id].get_vname();
  }

  std::map<std::pair<int, int>, def_use> get_struct_node() {
    return struct_node;
  }

private:
  std::map<int, def_use> du_node;
  std::map<std::pair<int, int>, def_use> struct_node;
};

struct Info {
  std::string varname;
  int def_line;
  int def_cl;
  Stmt *def_stmt;
  int defined;
  //tmp
  //global vardecl
  Decl *def_decl;

  int block_id;
  int var_id;
  SourceLocation location;
};

struct struct_Info {
  std::string varname;
  std::string type;
  int def_line;
  int def_cl;
  Stmt *def_stmt;
  int defined;
  //tmp
  //global vardecl
  Decl *def_decl;

  int block_id;
  std::pair<int, int> var_id;
  SourceLocation location;
};

//std::map<int, Info> global_def;


#endif //DEF_USE_H
