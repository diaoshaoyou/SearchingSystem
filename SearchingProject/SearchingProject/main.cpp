#define InstErr "#Error: invalid instruction!"
#define OverflowErr "#Error: instruction is too long!"
#include "VSM.h"
#include "BoolMatch.h"
#include "PhraseMatch.h"
using namespace std;
enum {
	Exit = 0,
	Bool_Match = 1,
	Wild_Match = 2,
	Correct = 3,
	TopK = 4,
	Phrase_Match = 5,
	Synonym = 6
};
void Readin(vector<string>& inputList);
void PrintResult(vector<int>& res);
void Up2Low(string& str);
Preprocess prework = Preprocess();
BoolMatch boolmatch = BoolMatch();
PhraseMatch phrasematch = PhraseMatch();
int op = 0;
int main() {
	prework.Run();
	int exit = 0;
	vector<string> inputList;
	//while (1) {
	cout << "#Choose your instruction:" << endl << "0. Exit 1.Bool match  2.Wild match  3.Spelling correct 4.TopK match 5.Phrase match  6.Synonym match" << endl << endl;
	//	cin >> op;
	//	cout << "#Input your query: ";//query ends with ;
	Readin(inputList);
	switch (op) {
	case Bool_Match:
		boolmatch.Run(inputList, prework.invertIdx);
		PrintResult(boolmatch.resDoc);
		break;
	case Wild_Match:
		break;
	case Correct:
		break;
	case TopK:
		break;
	case Phrase_Match:
		phrasematch.Run(inputList, prework.invertIdx);
		PrintResult(phrasematch.resDoc);
		break;
	case Synonym:
		break;
	case Exit:
		exit = 1;
		break;
	default:
		cout << InstErr << endl;
		break;
	}
	//if (exit) break;
//}
}
void Readin(vector<string>& inputList) {
	string str;
	fstream query("../Query.txt", fstream::in);
	query >> str;
	op = atoi(str.c_str());
	while (query >> str) {
		if (!str._Equal("NOT") && !str._Equal("OR") && !str._Equal("AND")) {
			Up2Low(str);//to lower case
		}
		inputList.emplace_back(str);
	}
}
void Up2Low(string& str) {
	int L = str.length();
	for (int i = 0; i < L; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] += 32;
		}
	}
}
void PrintResult(vector<int>& res) {
	if (res.at(0) == ERROR) {
		cout << InstErr << endl;
		return;
	}
	else if (res.at(0) == PHRASE_OVERFLOW) {
		cout << OverflowErr << endl;
		return;
	}
	cout << "#Satisfying documents:" << endl;
	if (res.at(0) == EMPTY) {
		cout << "none";
	}
	else {
		for (int id : res) {
			cout << "doc " << id << "  ";
		}
	}
	cout << endl;
}