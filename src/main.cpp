#include "read_write.h"
#include "searchIOF.h"
#include "findfunction.h"
#include "datapathGen.h"
#include "allnodes.h"


void print(vector<string> &c)
{
	bool firstC=true;
	for (int i = 0; i < c.size(); i++)
	{
		cout << c[i];
		if (c[i] == ")" && firstC == true)
		{
			cout << endl;
			firstC = false;
		}
		//cout << " " << c[i];
		if (c[i] == ";")
			cout << endl;
		
	}
}
string remove_txt(string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str.at(i) == '.')
			return str.substr(0, i);
	}
	try {
		throw 1;
	}
	catch (int x) {
		cout << "Incorrect file format";
	}
	return "0";
}

int main(int argc, char*argv[]) {
	vector<string> b;			//read vector
	vector<string> c;			//write vector
	readvector(b,argv[1]);	
	
	datanode node[60];
	b.push_back("\0");
	c.push_back("module ");
	string str = remove_txt(argv[1]);	//find the name of module
	c.push_back(str);
	int nsize=findallnodes(b,node);
	printnode(c, node, nsize);		//print the node..can be removed
	datapathgenerator(b, node);		//to generate data path  critical value
	findfunction(b, c, node); 		// finds the add,sub... functions and converts them to module in verilog
	
	c.push_back("endmodule");
	writevector(c, argv[2]);
	//print(c);
	return 0;
}




