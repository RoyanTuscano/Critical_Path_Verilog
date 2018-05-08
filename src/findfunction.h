#ifndef findfun
#define findfun
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<math.h>
using namespace std;
extern int nodepointer;
extern int add_ref ;
extern int sub_ref ;
extern int mul_ref ;
extern int com_ref;
extern int mux_ref ;
extern int reg_ref ;
extern int shr_ref;
extern int shl_ref;
extern int div_ref;
extern int inc_ref;
extern int dec_ref;
extern int mod_ref;

extern int refresh(string str, vector <string>&c, int start);

int check_signed(string name_node, struct datanode *node);
int node_width(string name_node, struct datanode *node, string type_name);
void XDelayComp(vector<string> &c, string *str, string name, int ref, struct datanode *node);
void findfunction(vector<string> & b, vector<string> &c, struct datanode *node);
bool searchcomp(string input1, string input2, int i, vector<string> & b, string gle, string *op);
int searchwire(vector<string> & c, string *value, string *pvalue);
void comparator(string *gtn, string *ltn, string *eqt, vector<string> & b, vector<string> & c, int i);


#endif



