#include "findfunction.h"
#include "allnodes.h"
 int add_ref=1;
 int sub_ref=1;
 int mul_ref=1;
 int com_ref=1;
 int mux_ref=1;
 int reg_ref=1;
 int shr_ref = 1;
 int shl_ref = 1;
 int div_ref = 1;
 int inc_ref = 1;
 int dec_ref = 1;
 int mod_ref = 1;

 int check_signed(string name_node, struct datanode *node)
 {
	 for (int i = 0; i < nodepointer; i++)
	 {
		 if (node[i].name == name_node)
		 {
			 
			 if (node[i].type.at(0) == 'I')
				 return 0;
			 else
				 return node[i].width;
		 }
	 }
	 return 0;
 }

 int node_width(string name_node, struct datanode *node, string type_name)	//tyoe_namme is passed to just output it in case of error
 {
	 int l = 0;
		 for (int i = 0; i < nodepointer; i++)
		 {
			 if (node[i].name == name_node)
			 {
				 return node[i].width;
			 }
		 }
		 
		 if (name_node != "gtn" && name_node !="ltn" && name_node!="eqt" && name_node != "Clk" && name_node!="Rst" && name_node !="1") {
			 try {
				 throw (name_node);
			 }
			 catch (string name_node) {
				 cout << " Node : "<<name_node<<" Not Declared of type "<<type_name;
			 }
		 }
		  
		
 }
 
 
 void XDelayComp(vector<string> &c, string *str, string name, int ref,struct datanode *node)
 {
	 int i = 0;
	 int width = 0;
	 int j = 0;
	 int nwidth=0;
	 int check_sign[2] = {0,0};
	 for (j = 0; str[j + 1] != "\0"; j++);
	 if (name == "REG")					
	 {
		width = node_width(str[j], node,"output");			//check the what type of output it is and than enter its width
	 }
	 else
	 {
		width = node_width(str[j], node, "wire");
	 }
	 if (name == "COMP")
	 {
		 width = node_width(str[j - 4], node, "wire");
	 }

	 for (i = 0; i < 2; i++)						//check if inout or outut are signed or unsigned
	 {
		 check_sign[i] = check_signed(str[i], node);
	 }
	 string w = to_string(width);
	 c.push_back(name);
	 c.push_back(" #(.WIDTH(");
	 c.push_back(w);
	 c.push_back(")) ");
	 c.push_back(name);
	 w = to_string(ref);
	 c.push_back(w);
	 c.push_back(" (");
	 for ( i = 0; str[i+1] != "\0"; i++)
	 {
		 nwidth=node_width(str[i],node, "input");			// to check if all the variables exist in the code.
		 if (check_sign[0]+check_sign[1] == 0)				//to check if the variable is signed or unsigned.
		 {
			 c.push_back(str[i]);					//if all is signed than just push
		 }
		 else
		 {
			 c.push_back("{");					//this operations are performed incase any inout is unsigned
			 if (width - nwidth > 0)
			 {
				 
				 w = to_string(width - nwidth);
				 c.push_back(w);
				 
			 }
			 else						 //all the calculations for inputing the widths of the signed and unsigned variables
			 {
				int Fnwidth = node_width(str[0], node, "input");
				int Snwidth = node_width(str[1], node, "input");
				if (Fnwidth > Snwidth && i==1)
				{
					c.push_back(to_string(Fnwidth - Snwidth));
				}
					else if (Fnwidth < Snwidth && i == 0)
				{
					c.push_back(to_string(Snwidth - Fnwidth));
				}
				else if(Fnwidth == Snwidth)
				{
					c.push_back("1");
				}
				else
				{
					c.push_back("0");
				}
				
				 
			 }
			 c.push_back("'");
			 c.push_back("b");
			 if (check_sign[i] == 0)
				 c.push_back("1");
			 else
				 c.push_back("0");
			 c.push_back(",");
			 c.push_back(str[i]);
			 c.push_back("}");
			 
		 }
		
		 c.push_back(",");

	 }
	 
	 c.push_back(str[i]);

	 c.push_back(")");
	 c.push_back(";");
 }

void findfunction(vector<string> & b, vector<string> &c, struct datanode *node)
{

	int i = 0;
	while (i < b.size()) {
		if (b[i] == "=")
		{
			if (b[i + 2] == "+")						//check for type of operation
			{

				string str[] = { b[i + 1],b[i + 3],b[i - 1],"\0" };
				if (b[i + 3] == "1")
				{	
					XDelayComp(c, str, "INC", inc_ref++,node);	//pass the inputs and outputs to write in tje vector
				}
				else
				{
					XDelayComp(c, str, "ADD", add_ref++,node);
				}
				i = i + 4;
			}
			else if (b[i + 2] == "-")
			{
				string str[] = { b[i + 1],b[i + 3],b[i - 1],"\0" };
				if (b[i + 3] == "1")
				{
					XDelayComp(c, str, "DEC", dec_ref++, node);
				}
				else
				{
					XDelayComp(c, str, "SUB", sub_ref++, node);
				}
				i = i + 4;
			}
			else if (b[i + 2] == ">" || (b[i+2] == "==") || (b[i + 2] == "<"))
			{
			
				string *gtn = new string("gtn"); 
				string *ltn = new string("ltn");
				string *eqt = new string("eqt");
				comparator(gtn, ltn, eqt, b,c,i);	//this function is used to search for all the values of the compatatior functions

				string str[] = { b[i + 1],b[i + 3],*gtn,*ltn,*eqt,"\0" };

				XDelayComp(c, str, "COMP", com_ref++, node);
				delete gtn;
				delete ltn;
				delete eqt;
				i = i + 4;
			}

			else if (b[i + 2] == "*")
			{
				string str[] = { b[i + 1],b[i + 3],b[i - 1],"\0" };
				XDelayComp(c, str, "MUL", mul_ref++, node);
				i = i + 4;
			}
			else if (b[i + 2] == "?")
			{
				
				string str[] = { b[i + 1],b[i + 3],b[i + 5],b[i-1],"\0" };
				XDelayComp(c, str, "MUX2x1", mux_ref++, node);
				i = i + 6;
			}

			else if (b[i + 2] == "\0" || b[i + 3] == "=")
			{
				if (reg_ref == 1)
				{
					int j=refresh("output",c,0);
					
				c.insert(c.begin()+j, "");
				c.insert(c.begin()+j+1, "");
				}
				string str[] = { "Clk","Rst",b[i + 1],b[i - 1],"\0" };
				XDelayComp(c, str, "REG", reg_ref++, node);
				i = i + 2;

			}
			else if (b[i + 2] == "<<")
			{

				string str[] = { b[i + 1],b[i + 3],b[i - 1],"\0" };
				XDelayComp(c, str, "SHL", shl_ref++, node);
				i = i + 2;
			}
			else if (b[i + 2] == ">>")
			{

				string str[] = { b[i + 1],b[i + 3],b[i - 1],"\0" };
				XDelayComp(c, str, "SHR", shr_ref++, node);
				i = i + 2;
			}
			else if (b[i + 2] == "/")
			{

				string str[] = { b[i + 1],b[i + 3],b[i - 1],"\0" };
				XDelayComp(c, str, "DIV", div_ref++, node);
				i = i + 2;
			}
			else if (b[i + 2] == "%")
			{

				string str[] = { b[i + 1],b[i + 3],b[i - 1],"\0" };
				XDelayComp(c, str, "MOD", mod_ref++, node);
				i = i + 2;
			}
			else
			{
				cout << "incorrect operator";
				exit(EXIT_FAILURE);
			}

		}
		i++;
	}

}




bool searchcomp(string input1, string input2, int i, vector<string> & b,string gle,string *op)
{
	for (int j = i+4; j < b.size()-5; j++)
	{
		if (b[j] == "=")
		{
			if (b[j + 1] == input1 && b[j + 3] == input2 && b[j + 2] == gle)
			{
				*op = b[j - 1];
				b.erase(b.begin() + (j - 1), b.begin() + (j + 3));
				return false;
			}
		}
		
	}
	return true;
	
}

int searchwire(vector<string> & c,string *value, string *pvalue)
{
	for (int i = 0;i< c.size(); i++)
	{
		if (c[i] == *pvalue)
		{
			c.insert(c.begin() + (i + 1), *value);
			c.insert(c.begin() + (i + 1), ",");
			return 0;
		}
			
	}
}

void comparator(string *gtn, string *ltn, string *eqt, vector<string> & b, vector<string> & c,int i)
{
	
	if (b[i + 2] == ">")
	{
		*gtn = b[i - 1];
		if (searchcomp(b[i + 1], b[i + 3], i, b, "<", ltn))			// search for lesser than condition
			searchwire(c, ltn,gtn);
		if( searchcomp(b[i+1], b[i + 3], i, b, "==",eqt))			//search for equal to condition
			searchwire(c, eqt, gtn);
	}
	else if (b[i + 2] == "<")
	{
		*ltn = b[i - 1];
		if (searchcomp(b[i+1], b[i + 3], i, b, ">", gtn))			// search for greater than condition
			searchwire(c, gtn, ltn);
		if (searchcomp(b[i + 1], b[i + 3], i, b, "==", eqt))			//search for equal to condition
			searchwire(c, eqt, ltn);
	}
	else if (b[i + 2] == "==")
	{
		*eqt = b[i - 1];
		if (searchcomp(b[i + 1], b[i + 3], i, b, ">", gtn))			// search for greater than condition
			searchwire(c, gtn, eqt);
		if (searchcomp(b[i + 1], b[i + 3], i, b, "<", ltn))			// search for lesser than condition
			searchwire(c, ltn, eqt);
	}

}

























