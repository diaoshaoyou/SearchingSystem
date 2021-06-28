#pragma once
#include "global.h"
#include "Preprocess.h"


enum {

};

class WildMatch {
public:
	vector<int> resDoc;//result doc ID
	vector<node> resWord;//result word
	WildMatch() { };
	~WildMatch() {};
	//void Run(vector<string>& inputList, TwogramNode Kgramhead);//start Wild Match
	void Run(vector<string>& inputList, TwogramNode KgramHash[]);//start Wild Match
private:
	vector<string> KgramList;//save the word between *

	bool checkSyntax(vector<string>& inputList);//check basic syntax error

	void search(vector<string>& inputList, TwogramNode KgramHash[]);//search database using post expression
	void wordtokgram(string word, TwogramNode KgramHash[]);//turn word into 2-gram
	TwogramNode FindKgramNode(string word, TwogramNode KgramHash[]);


};