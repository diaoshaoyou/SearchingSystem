#pragma once
#define ERROR -9999
#define EMPTY -99
#include "Preprocess.h"
#include <set>
#include <algorithm>

enum {

};

struct ScoreNode {
	double score;
	int docID;
	ScoreNode* LeftNode;
	ScoreNode* RightNode;
};


class TOPK {
public:
	vector<int> resDoc;//result doc ID
	//exception: resDoc[0]==EMPTY, no satisfying doc; resDoc[0]==ERROR, error

	TOPK() { };
	~TOPK() {};
	void Run(vector<string>& inputList, vector<pair<string, double> >* vectorspace);//start bool search

private:
	int k;//Top k
	int inputID;//input's docID
	ScoreNode* MxHeapHead;
	bool checkSyntax(vector<string>& inputList, vector<pair<string, double> >* vectorspace);//check basic syntax error

	void search(vector<pair<string, double> >* vectorspace);//search database using post expression
	double FindTFIDF(vector<pair<string, double>>vector, string word);
	void InsertHeap(int docID, double score);
	ScoreNode* createScoreNode(int docID, double score);
	int PopHeap();
};