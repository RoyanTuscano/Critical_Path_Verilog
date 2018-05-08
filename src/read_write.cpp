#include "read_write.h"
bool check_string(string stra)
{
	for (int i = 0; i < stra.length(); i++)
	{
		if (stra.at(i) != ' ')
		{
			return true;
		}
	}
	return false;
}
void readvector(vector<string>& c,string filename)
{
	string str,stra;
	int ignore=0;
	int begin = 0;
	int l=0;
	int i = 0;
	int j = 0;
	ifstream b;
	b.open(filename);
	if (!b.is_open()) { cout << "\nfailure to read file" << endl; }
	while (getline(b, str))
	{
			if (str.find("//") != string::npos)
			{
				str = str.substr(0, str.find("//"));
			}
			for (i = 0; i < str.length(); i++)
			{
				l++;
				if (str.at(i) == ' ')
				{
					stra = str.substr(begin, l - 1);
					if (check_string( stra)) {
						c.push_back(stra);
						begin = i + 1;
						l = 0;
					}
					
				}
			}
			stra = str.substr(begin);
			if (check_string(stra))
			{
				c.push_back(stra);
			}
			l = 0;
			begin = 0;


	}
	b.close();

	return;
}
void writevector(vector<string> &c, string filename)
{
	ofstream myfile;
	myfile.open(filename);
	if (!myfile.is_open()) { cout << "\nfailure to write file" << endl; }
	bool firstC = true;
	for (int i = 0; i < c.size(); i++)
	{
		myfile << c[i];
		if (c[i] == ")" && firstC == true)
		{
			myfile << endl;
			firstC = false;
		}
		if (c[i] == ";")
			myfile << endl;

	}

}
