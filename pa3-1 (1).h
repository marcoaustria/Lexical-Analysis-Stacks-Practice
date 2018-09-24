/*
 * pa3.h


 *
 *  Created on: Nov 20, 2016
 *      Author: Marco Austria
 */

#ifndef PA3_H_
#define PA3_H_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Stack {
public:
	void push(string line);
	string pop();
	int size();
	string top();
private:
	vector<string> stackVector;
};

void Stack::push(string line) {
	stackVector.push_back(line);
}

string Stack::pop() {
	if (stackVector.empty()) {
		return "";
	}
	string temp = stackVector[stackVector.size() - 1];
	stackVector.pop_back();
	return temp;
}

string Stack::top() {
	if (stackVector.empty()) {
		return "";
	}
	return stackVector[stackVector.size() - 1];
}

int Stack::size() {
	if (stackVector.empty()) {
		return 0;
	}
	return stackVector.size();
}

//pops lineStack, finds keywords, finds identifiers, etc
//prints everything out
void compile(Stack lineStack1) {

	string keywords = "";
	string identifier = "";
	string constant = "";
	string operators = "";
	string delimiter = "";
	string syntaxErrors = "";

	string testString = "";
	string testConstant = "";
	string testOperators = "";
	string testOperators1 = "";
	string testOperators2 = "";
	string testOperators3 = "";
	bool isOperator = false;

	int loopDepth = 0;

	int forNum = 0;
	int beginNum = 0;
	int endNum = 0;

	Stack keywordStack;
	Stack loopStack;

	//finds all keywords, identifiers, constants,
	//operators, delimiters, and syntax errors
	while (lineStack1.size() != 0) {

		//get top string on stack
		string topLine = lineStack1.pop();

		int lineSize = topLine.length();

//get each character from top string and evaluate
		for (int i = 0; i < lineSize; i++) {

			char testChar = topLine.at(i);

			//if a letter
			//evaluate if a keyword
			if ((((testChar >= 'A') && (testChar <= 'Z'))
					|| ((testChar >= 'a') && (testChar <= 'z')))) {

				if (topLine.substr(i, 3) == "FOR") {
					keywordStack.push("FOR");
					//first time entry
					if (forNum == 0) {
						keywords += "FOR ";
					}
					forNum++;
					i = i + 2;
					continue;
				} else if (topLine.substr(i, 5) == "BEGIN") {
					keywordStack.push("BEGIN");
					//first time entry
					if (beginNum == 0) {
						keywords += "BEGIN ";
					}
					i = i + 4;
					beginNum++;
					continue;
				} else if (topLine.substr(i, 3) == "END") {
					keywordStack.push("END");
					//first time entry
					if (endNum == 0) {
						keywords += "END ";
					}
					i = i + 2;
					endNum++;
					continue;
				}

				//to be evaluated later
				else {
					testString += testChar;
				}
			} //if

			//means the word has ended because the next variable is not a letter
			//or end of the list and need to evaluate testString
			if ((!(((testChar >= 'A') && (testChar <= 'Z'))
					|| ((testChar >= 'a') && (testChar <= 'z')))
					|| (i == lineSize - 1)) && testString != "") {


				if (isdigit(testChar) && testString != "") {
					testString += testChar;
				}

				//all lower case identifier
				for (string::size_type i = 0; i < testString.length(); i++) {

					//end of the word and all letters lowercase
					if ((i == testString.length() - 1)
							&& (islower(testString[i]))) {

						//found, already in list
						if (identifier.find(testString) != string::npos) {
							testString = "";
							break;
						}
						//not found in list, ok to add
						else {
							identifier += testString += " ";
							testString = "";
							break;
						}
					}

					if (islower(testString[i])) {
						continue;
					} else {
						break;
					}
				}

				//potential syntax error like identifier with capital letter or number
				//or keyword with lowercase letter
				//found, already in list
				if (syntaxErrors.find(testString) != string::npos) {
					testString = "";
				}
				//not found in list, ok to add
				else {
					syntaxErrors += testString += " ";
					testString = "";
					continue;
				}

			} //if

			//find constants
			if (isdigit(testChar)) {
				testConstant += testChar;
			} else {
				//found, already in list
				if (constant.find(testConstant) != string::npos) {
					testConstant = "";
				}
				//not found in list, ok to add
				else {
					constant += testConstant += " ";
					testConstant = "";
				}
			}

			//find delimiters
			if (testChar == ';' || testChar == ',') {
				//found, already in list
				if (delimiter.find(testChar) != string::npos) {
				}
				//not found in list, ok to add
				else {
					delimiter += testChar;
					delimiter += " ";
				}
			}

			//find operators
			string testOperatorArray = "= ! % - * / +";
			string testOperatorArray2 = "++ == --";
			string testOperatorArray3 = "+= -= *= /= != %=";

			//did find in array, which is good
			if (testOperatorArray.find(testChar) != string::npos
					&& testChar != ' ') {
				testOperators += testChar;
				isOperator = true;
			} else if ((testOperators.length() != 0)
					&& testOperatorArray.find(testOperators) != string::npos) {
				testOperators1 += testOperators;
				testOperators = "";
				isOperator = false;
			} else if ((testOperators.length() != 0)
					&& testOperatorArray2.find(testOperators) != string::npos) {
				testOperators2 += testOperators;
				testOperators = "";
				isOperator = false;
			} else if ((testOperators.length() != 0)
					&& testOperatorArray3.find(testOperators) != string::npos) {
				testOperators3 += testOperators;
				testOperators = "";
				isOperator = false;
			} else {
				isOperator = false;

				//find syntax errors
				if (!(((testChar >= 'A') && (testChar <= 'Z'))
						|| ((testChar >= 'a') && (testChar <= 'z')))
						&& !isdigit(testChar) && testChar != ';'
						&& testChar != ',' && testChar != ' ' && testChar != ')'
						&& testChar != '('&&(testOperators.length()!=0)&&(!(isblank(testChar)))) {

					//not already in the list
					if (syntaxErrors.find(testOperators) == string::npos) {
						syntaxErrors += testOperators += " ";
						testOperators = "";
					}
					if (syntaxErrors.find(testChar) == string::npos) {
						syntaxErrors += testChar;
						syntaxErrors += " ";
					}
				}

			}
			if ((isOperator == false)
					&& (testOperators1.length() != 0
							|| testOperators2.length() != 0
							|| testOperators3.length() != 0)) {

				if (testOperators1.length() != 0) {
					if (operators.find(testOperators1) != string::npos) {
						testOperators1 = "";
					} else {
						operators += testOperators1 += " ";
						testOperators1 = "";
					}
				}
				if (testOperators2.length() != 0) {
					if (operators.find(testOperators2) != string::npos) {
						testOperators2 = "";
					} else {
						operators += testOperators2 += " ";
						testOperators2 = "";
					}
				}
				if (testOperators3.length() != 0) {
					if (operators.find(testOperators3) != string::npos) {
						testOperators3 = "";
					} else {
						operators += testOperators3 += " ";
						testOperators3 = "";
					}
				}
				testOperators = "";
			}

		} //for
	} //while

	//find loop depth
	string evaluateKeyword = "";
	while (true) {

		if (keywordStack.size() == 0 && evaluateKeyword == "") {
			break;
		}

		if (evaluateKeyword == "") {
			evaluateKeyword = keywordStack.pop();
		}

		//starts FOR
		if (evaluateKeyword == "FOR") {
			loopStack.push("FOR");
			evaluateKeyword = "";
		}
		//FOR then BEGIN
		else if (loopStack.top() == "FOR" && evaluateKeyword == "BEGIN") {
			loopStack.push("BEGIN");
			evaluateKeyword = "";
		}
		//BEGIN then END
		else if (evaluateKeyword == "END" && loopStack.top() == "BEGIN") {
			//pop out BEGIN
			loopStack.pop();
			//pop out FOR
			loopStack.pop();
			loopDepth++;
			evaluateKeyword = "";

		} else if (evaluateKeyword == "END"
				&& (loopStack.top() == "FOR" || loopStack.top() == "END")) {
			//pop bad keyword
			loopStack.pop();
			//will reevaluate
			evaluateKeyword = "END";
		} else {
			evaluateKeyword = "";
		}

	} //while

	//print results
	cout << "The depth of nested loop(s) is " << loopDepth << endl;
	cout << "\n";
	cout << "Keywords: " << keywords << endl;
	cout << "Identifier: " << identifier << endl;
	cout << "Constant: " << constant << endl;
	cout << "Operators: " << operators << endl;
	cout << "Delimiter: " << delimiter << endl;
	cout << "\n";
	cout << "Syntax Error(s): " << syntaxErrors << endl;

}

#endif /* PA3_H_ */
