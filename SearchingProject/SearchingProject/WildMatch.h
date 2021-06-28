#pragma once
#include "global.h"
#include "Preprocess.h"


enum {

};

class WildMatch {
public:
	vector<int> resDoc;//result doc ID
	//exception: resDoc[0]==EMPTY, no satisfying doc; resDoc[0]==ERROR, error
	vector<node> resWord;//result word
	WildMatch() { };
	~WildMatch() {};
	//void Run(vector<string>& inputList, TwogramNode Kgramhead);//start Wild Match
	void Run(vector<string>& inputList, TwogramNode KgramHash[]);//start Wild Match
private:
	vector<string> KgramList;//save the word between *

	bool checkSyntax(vector<string>& inputList);//check basic syntax error


	//void search(vector<string>& inputList,TwogramNode Kgramhead);//search database using post expression
	void search(vector<string>& inputList, TwogramNode KgramHash[]);//search database using post expression
	//void wordtokgram(string word, TwogramNode Kgramhead);//turn word into 2-gram
	void wordtokgram(string word, TwogramNode KgramHash[]);//turn word into 2-gram
	//TwogramNode FindKgramNode(string word, TwogramNode Kgramhead);
	TwogramNode FindKgramNode(string word, TwogramNode KgramHash[]);

	bool CheckNodeExist(string word, vector<node>& res);
	void CheckRes();
	void ResWordtoResDoc();


};