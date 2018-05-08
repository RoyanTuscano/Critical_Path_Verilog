#ifndef dpgene
#define dpgene
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<math.h>
using namespace std;
extern int nodepointer;
extern float  XCompDelay[12][6];
extern float critical_delay;

extern void printNode(struct datanode *node, int dsize);
extern int node_width(string name_node, struct datanode *node, string type_name);
void datapathgenerator(vector<string> & b, struct datanode *node);
void printNode(struct datanode *node, int dsize);
float checkinputdelay(struct datanode *node, string *input, int dsize);
void calculateDelayComp(struct datanode *node, int dsize, int i, string *str, string output, int matrixrow,string operation);
void datapathgenerator(vector<string> & b, struct datanode *node);
float maxdelay(struct datanode *node, int dsize);


#endif
