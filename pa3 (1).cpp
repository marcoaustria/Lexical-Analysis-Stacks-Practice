//Marco Austria 11/20/16

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "pa3.h"
using namespace std;

int main() {

	string fileName;
	string line;
	ifstream myfile;
	Stack lineStack;

	//requests filename, will repeat if not found
	while (true) {
		cout << "Please enter the name of the input file: " << endl;

		cin >> fileName;
		cout<<"\n";

		myfile.open(fileName.c_str());

		if (myfile.is_open()) {

			//push each line of the file to stack
			while (getline(myfile, line)) {

				lineStack.push(line);
			}
			myfile.close();
			break;

		} else {
			cout << "Unable to open file. Please choose another.\n" << endl;
			continue;
		}
	}

//pops lineStack, finds keywords, finds identifiers, etc
//prints everything out
	compile(lineStack);

	return 0;
}
