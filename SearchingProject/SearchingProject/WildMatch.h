#pragma once
#define ERROR -9999
#define EMPTY -99
#include "Preprocess.h"
#include <set>
#include <algorithm>

enum {

};

class WildMatch {
public:
	vector<int> resDoc;//result doc ID
	//exception: resDoc[0]==EMPTY, no satisfying doc; resDoc[0]==ERROR, error
	vector<node> resWord;//result word
	WildMatch() { };
	~WildMatch() {};
	void Run(vector<string>& inputList, TwogramNode Kgramhead);//start bool search
	
private:
	vector<string> KgramList;//save the word between *

	bool checkSyntax(vector<string>& inputList);//check basic syntax error
	

	void search(vector<string>& inputList,TwogramNode Kgramhead);//search database using post expression
	void wordtokgram(string word, TwogramNode Kgramhead);//turn word into 2-gram
	TwogramNode FindKgramNode(string word, TwogramNode Kgramhead);

	bool CheckNodeExist(string word,vector<node>& res);
	void CheckRes();
	void ResWordtoResDoc();
	void DeleteWord(string word);
	bool CheckIDExist(int ID);
	
};