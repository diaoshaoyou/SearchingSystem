#include "global.h"
#include "Search.h"
#include "Preprocess.h"
using namespace std;

void Readin(vector<string>& inputList);
void PrintResult(vector<int>& res);
void Up2Low(string& str);
Preprocess prework = Preprocess();
Search mysearch = Search();
int op = 0;
int main() {
	clock_t startT, endT;
	prework.Run();
	int exit = 0;
	vector<string> inputList;
	while (1) {
		cout << "#Choose your instruction:" << endl << "0. Exit 1.Bool match  2.Wild match  3.Spelling correct 4.TopK match 5.Phrase match  6.Synonym match" << endl << endl;
		cin >> op;
		cout << "#Input your query: ";//query ends with ;
		Readin(inputList);
		cout << "#searching..." << endl;
		startT = clock();
		switch (op) {
		case Bool_Match:
			mysearch.boolmatch->Run(inputList, prework.BZhash);
			PrintResult(mysearch.boolmatch->resDoc);
			break;
		case Wild_Match:
			mysearch.wildmatch->Run(inputList, prework.KgramHash);
			PrintResult(mysearch.wildmatch->resDoc);
			break;
		case Correct:
			mysearch.spellingcorrect->Run(inputList, prework.KgramHash);
			break;
		case TopK:
			mysearch.topk->Run(inputList, prework.vectorSpace);
			PrintResult(mysearch.topk->resDoc);
			break;
		case Phrase_Match:
			mysearch.phrasematch->Run(inputList, prework.BZhash);
			PrintResult(mysearch.phrasematch->resDoc);
			break;
		case Synonym:
			mysearch.synextend->Run(inputList, prework.BZhash);
			break;
		case Exit:
			exit = 1;
			break;
		default:
			cout << InstErr << endl;
			break;
		}
		if (exit) break;
		endT = clock();
		cout << "search cost " << (double)(endT - startT) / CLOCKS_PER_SEC << "s" << endl << endl;
	}

}
void Readin(vector<string>& inputList) {
	string str;
	inputList.clear();
	//fstream query("../Query.txt", fstream::in);
	//query >> str;
	//op = atoi(str.c_str());
	while (cin >> str) {
		if (str._Equal(";")) break;
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
	cout << "#Satisfying documents:" << res.size() << endl;
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