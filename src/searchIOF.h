#ifndef searchy
#define searchy
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std; 

void printnode(vector<string> &c, struct datanode *node, int nsize);
void print_IO(struct datanode *node, vector<string> &c, int nsize);
void print_IOWG(struct datanode *node, vector<string> &c, int nsize);
int vector_search(string str, vector <string> &c, int start);
int refresh(string str, vector <string>&c, int start);

#endif

