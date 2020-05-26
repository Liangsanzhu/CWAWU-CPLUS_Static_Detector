#ifndef DEF_USE_H
#define DEF_USE_H

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
    defined = false;
    block_id = -1;
  }

  void add_defuse_ln(int def, int use) {
    std::pair<int, int> pr(def, use);
    defuse_ln = pr;
  }

  std::pair<int, int> get_defuse_ln() {
    return defuse_ln;
  }

  void add_funcname(std::string funcname) {
    func_name = funcname;
  }

  std::string get_funcname() {
    return func_name;
  }

  void add_defined(bool d) {
    defined = d;
  }

  bool get_defined() {
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
    std::cout << defuse_ln.first << " " << defuse_ln.second << "\n";
    std::cout << "Belong to Function: " << func_name << "\n";
    if (defuse_stmt.first != nullptr)
      std::cout << defuse_stmt.first->getStmtClassName() << "\n";
    if (defuse_stmt.second != nullptr)
      std::cout << defuse_stmt.second->getStmtClassName() << "\n";
    std::cout << block_id << "\n";
  }
 

private:
  std::pair<int, int> defuse_ln;
  std::string func_name;
  //when ln_col == 0
  std::pair<Stmt *, Stmt *> defuse_stmt;
  bool defined;
  int block_id;

};

class def_use {
public:

  def_use() {
    ;
  }

  void add_defuse_ln(int idx, SourceLocation sl, int def_ln, int use_ln) {
    std::pair<int, SourceLocation> pr(idx, sl);
    du[pr].add_defuse_ln(def_ln, use_ln);
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
    std::cout << var_name << "  ";
    std::cout << type << "\n";
    for (auto i = du.begin(); i != du.end(); ++i) {
      std::cout << (*i).first.first << "\n";
      std::cout << (*i).first.second.printToString(*scm);
      std::cout << "\n";
      (*i).second.output_element();
    }
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

  
  void add_var(int id, std::string var, std::string type) {
    du_node[id] = def_use();
    du_node[id].add_varname(var);
    du_node[id].add_type(type);
  }

  void add_defuse_ln(int id, int idx, SourceLocation sl, int def, int use) {
    du_node[id].add_defuse_ln(idx, sl, def, use);
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

private:
  std::map<int, def_use> du_node;
};

#endif //DEF_USE_H