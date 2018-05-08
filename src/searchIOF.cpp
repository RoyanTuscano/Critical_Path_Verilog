#include "searchIOF.h"
#include "allnodes.h"

void printnode(vector<string> &c, struct datanode *node,int nsize)
{
	int i = 0;
	c.push_back(" ( ");
	print_IO(node, c,nsize);
	c.pop_back();
	c.push_back(")");
	c.push_back("input Clk, Rst");
	print_IOWG(node, c, nsize);
	c.push_back(";");

}


void print_IO(struct datanode *node, vector<string> &c, int nsize)
{

	for (int i = 0; i < nsize; i++)
	{
		if (node[i].typeofIOWG == "input" || node[i].typeofIOWG == "output")
		{
			c.push_back(node[i].name);
			c.push_back(" , ");
		}
		}
		c.push_back("Clk , Rst");
		c.push_back(",");

	
}
void print_IOWG(struct datanode *node, vector<string> &c, int nsize)
{
	int initial = 0;

	int ref_search = 0;
	bool foundbefore=false;
	int temp = 0;
	c.push_back(";");
	for (int i = 0; i < nsize; i++)
	{
			initial = vector_search(node[i].typeofIOWG, c, ref_search);
			
			if (initial != 0)
			{
				ref_search = initial;
			}

			
			string str = node[i].type;
			if (str.substr(0, 1) == "U")
			{
				if (vector_search("signed", c,ref_search)!=0)
				{
					if (foundbefore = false){
						foundbefore = true;
					}
					else
					{
						c.push_back(";");
					}
					if(node[i].typeofIOWG=="ouput")
					c.push_back(node[i].typeofIOWG+" reg");
					else
						c.push_back(node[i].typeofIOWG);
					ref_search = refresh(node[i].typeofIOWG, c, ref_search + 1);
					c.push_back(" unsigned");
					c.push_back("[");
					c.push_back(to_string(node[i].width - 1));
					c.push_back(": 0 ]");
					c.push_back(node[i].name);
				}
				else
				{
					
						c.push_back(" , ");
						c.push_back(node[i].name);
					
					
				}
			}
			else if (str.substr(0, 1) == "I")
			{
				if (vector_search(to_string(node[i].width - 1), c, ref_search)!=0)
				{
					if (temp!=0) {
						c.push_back(";");
					}
					temp = 1;
						
					if (node[i].typeofIOWG == "ouput")
						c.push_back(node[i].typeofIOWG + " reg");
					else
						c.push_back(node[i].typeofIOWG);
					
						ref_search=refresh(node[i].typeofIOWG, c, ref_search+1);
						c.push_back(" signed");
						c.push_back("[");
						c.push_back(to_string(node[i].width - 1));
						c.push_back(": 0 ]");
						c.push_back(node[i].name);
				}
				else
				{
					c.push_back(" , ");
					c.push_back(node[i].name);
				}
			}

			
			

	}
		
	
}

int vector_search(string str, vector <string> &c,int start)
{
	int i = start;
	for (i = start; i < c.size(); i++)
	{
		if (str == c[i])
		{
			return 0;
			
		}
			
	}
	return i;
}
int refresh(string str, vector <string>&c, int start)
{
	int i = start;
	for (i = start; i < c.size(); i++)
	{
		if (str == c[i])
		{
			return i;
		}
	}
	return 0;
}



 

