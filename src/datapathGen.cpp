#include "datapathGen.h"
#include "allnodes.h"
float XCompDelay[12][6] = {
	{ 2.616 ,2.644 ,2.879 ,3.061 ,3.602, 3.966 },
	{ 2.704 ,3.713 ,4.924 ,5.638 ,7.270, 9.566 },
	{ 3.024 ,3.412 ,4.890 ,5.569 ,7.253 ,9.566 },
	{ 2.438 ,3.651 ,7.453 ,7.811 ,12.395 ,15.354 },
	{ 3.031 ,3.934 ,5.949 ,6.256 ,7.264 ,8.416 },
	{ 4.083 ,4.115 ,4.815 ,5.623 ,8.079 ,8.766 },
	{ 3.644 ,4.007 ,5.178 ,6.460 ,8.819 ,11.095 },
	{ 3.614 ,3.980 ,5.152 ,6.549 ,8.565 ,11.220 },
	{ 0.619 ,2.144 ,15.439 ,33.093 ,86.312 ,243.233 },
	{ 0.758 ,2.149 ,16.078 ,35.563, 88.142 ,250.583 },
	{ 1.792 ,2.218 ,3.111 ,3.471 ,4.347 ,6.200 },
	{ 1.792 ,2.218, 3.108, 3.701, 4.685, 6.503 }
};
float critical_delay = 0;

float maxdelay(struct datanode *node, int dsize)
{
	for (int j = 0; j<dsize; j++)
	{
		if (node[j].typeofIOWG == "output")
		{
			if (node[j].delay > critical_delay)
				critical_delay = node[j].delay;
		}
	}
	return critical_delay;
}

float checkinputdelay(struct datanode *node, string *input, int dsize)
{
	float maxdelay = 0;
	int i = 0;
	while (input[i] != "\0")
	{
		for (int j = 0; j<dsize; j++)
		{
			if (node[j].name == input[i])
			{
				if (node[j].delay > maxdelay)
					maxdelay = node[j].delay;
			}
		}
		i++;
	}
	return maxdelay;
}

void calculateDelayComp(struct datanode *node, int dsize, int i, string *str, string output, int matrixrow,string operation)
{
	float maxInputDelay = 0;
	int totaldelay;
	int width, j;
	maxInputDelay = checkinputdelay(node, str, dsize);

	for (j = 0; j < dsize; j++)
	{
	
		if (node[j].name == output)
		{
			width = node[j].width;
			if (matrixrow == 4)							//check for comparator
			{
				width= node_width(str[0],node,"wire");
			}
			if (operation == "=")
			{
				node[j].delay = maxInputDelay;
				if (node[j].delay > critical_delay)
				{
					critical_delay = node[j].delay;	
				}
				maxInputDelay = 0;
			}
			if (width == 8)
				node[j].delay = XCompDelay[matrixrow][2] + maxInputDelay;
			else if (width == 16)
				node[j].delay = XCompDelay[matrixrow][3] + maxInputDelay;
			else if (width == 32)
				node[j].delay = XCompDelay[matrixrow][4] + maxInputDelay;
			else if (width == 64)
				node[j].delay = XCompDelay[matrixrow][5] + maxInputDelay;
			else if (width == 1)
				node[j].delay = XCompDelay[matrixrow][0] + maxInputDelay;
			else if (width == 2)
				node[j].delay = XCompDelay[matrixrow][1] + maxInputDelay;
			break;
		}
	}
}



void datapathgenerator(vector<string> & b, struct datanode *node)
{
	int i = 0;
	int dsize = nodepointer;
	while (i < b.size()) {
		if (b[i] == "=")
		{
			if (b[i + 2] == "+")
			{
				if (b[i + 3] == "1")
				{
					string str[] = { b[i + 1],b[i + 3],"\0" };
					calculateDelayComp(node, dsize, i, str, b[i - 1], 10,b[i+2]);
					i = i + 4;
				}
				else
				{
					string str[] = { b[i + 1],b[i + 3],"\0" };
					calculateDelayComp(node, dsize, i, str, b[i - 1], 1,b[i+2]);
					i = i + 4;
				}
			}
			else if (b[i + 2] == "-")
			{
					if (b[i + 3] == "1")
				{
					string str[] = { b[i + 1],b[i + 3],"\0" };
					calculateDelayComp(node, dsize, i, str, b[i - 1], 11,b[i+2]);
					i = i + 4;
				}
				else
				{
					string str[] = { b[i + 1],b[i + 3],"\0" };
					calculateDelayComp(node, dsize, i, str, b[i - 1], 2,b[i+2]);
					i = i + 4;
				}
			}
			else if (b[i + 2] == ">" || (b[i + 2] == "==") || (b[i + 2] == "<"))
			{
				string str[] = { b[i + 1],b[i + 3],"\0" };
				calculateDelayComp(node, dsize, i, str, b[i - 1], 4,b[i+2]);
				i = i + 4;
			}
			else if (b[i + 2] == "*")
			{
				string str[] = { b[i + 1],b[i + 3],"\0" };
				calculateDelayComp(node, dsize, i, str, b[i - 1],3,b[i+2]);
				i = i + 4;
			}
			else if (b[i + 2] == "?")
			{
				string str[] = { b[i + 1],b[i + 3],b[i + 5],"\0" };
				calculateDelayComp(node, dsize, i, str, b[i - 1], 5,b[i+2]);
				i = i + 4;
			}

			else if (b[i + 2] == "\0" || b[i+3]=="=")
			{
				string str[] = { b[i + 1],"\0" };
				calculateDelayComp(node, dsize, i, str, b[i - 1], 0,"=");
				i = i + 2;
			}
			else if (b[i + 2] == "<<")
			{

				string str[] = { b[i + 1],b[i + 3],"\0" };
				calculateDelayComp(node, dsize, i, str, b[i - 1], 7,b[i+2]);
				i = i + 2;
			}
			else if (b[i + 2] == ">>")
			{

				string str[] = { b[i + 1],b[i + 3],"\0" };
				calculateDelayComp(node, dsize, i, str, b[i - 1], 6,b[i+2]);
				i = i + 2;
			}
			else if (b[i + 2] == "/")
			{

				string str[] = { b[i + 1],b[i + 3],"\0" };
				calculateDelayComp(node, dsize, i, str, b[i - 1], 8,b[i+2]);
				i = i + 2;
			}
			else if (b[i + 2] == "%")
			{

				string str[] = { b[i + 1],b[i + 3],"\0" };
				calculateDelayComp(node, dsize, i, str, b[i - 1], 9,b[i+2]);
				i = i + 2;
			}
		}
		i++;
	}
	//printNode(node, dsize);
	cout << endl << "Critical Delay : " << maxdelay(node,dsize)<<endl;
}
