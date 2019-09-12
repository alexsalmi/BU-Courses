#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <string.h>

using namespace std;



int main()
{
	std::string str1;
	std::string str2;
	cin >> str1;
	str2 = str1 + "1";
	ifstream Mary1,Mary2;
	Mary1.open(str2+".txt",ios::binary);
	Mary2.open(str1+".txt",ios::binary);
			cout << "   " << str1 << "\n";
			cout << "   " << str2 << "\n";
	
//---------- compare number of lines in both files ------------------//
	
//---------- compare two files line by line ------------------//
	char string1[256], string2[256];
	int j = 0;
	while(!Mary1.eof())
	{
		Mary1.getline(string1,256);
		Mary2.getline(string2,256);
		j++;
		if(strcmp(string1,string2) != 0)
		{
			cout << j << "-the strings are not equal" << "\n";
			cout << "   " << string1 << "\n";
			cout << "   " << string2 << "\n";
		}
	}
	
	return 0;
}