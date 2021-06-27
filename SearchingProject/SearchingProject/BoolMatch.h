#pragma once
#include "global.h"
#include "Preprocess.h"

enum {//priority: NOT>AND>OR
	_NOT=-1,
	_AND=-2,
	_OR = -3
};

class BoolMatch{
public:
	vector<int> resDoc;//result doc ID
	//exception: resDoc[0]==EMPTY, no satisfying doc; resDoc[0]==ERROR, error

	BoolMatch() { };
	~BoolMatch() {};
	void Run(vector<string>& inputList, WordNode invertIdx);//start bool search
	
private:
	vector< vector<int> > postExp;//post expression from query, store docIDs of one word	

	bool checkSyntax(vector<string>& inputList);//check basic syntax error

	void mid2Post(vector<string>& inputList, WordNode invertIdx);//mid expression to post expression
	void getDoc(string& word, WordNode invertIdx, vector<int>& ret);//given one word, get its docID
	
	void search();//search database using post expression
	void opOR_AND(vector<int>& docs1, vector<int>& docs2, int choice, vector<int>& tmpRes);//choice=0, OR; choice=1, AND. result=tmpRes
	void opNOT(vector<int> & docs, vector<int> & tmpRes);//docList - docs.  result=tmpRes
		
};