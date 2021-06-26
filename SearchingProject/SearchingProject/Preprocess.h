#pragma once
#define MAX_DOCNUM 21600
#define mx 100
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
using namespace std;

struct node{
	string WordVal;
	vector<int>* DocList;//vecotr[0]=docID, vector[i]=word's position in doc
	int DocNum;
	struct node* Next;
};
typedef node* WordNode;

typedef struct KgramNode {
	string Kgramword;
	struct node* wordList[mx];
	int wordNum;
	struct KgramNode* Next;
};
typedef KgramNode* TwogramNode;

class Preprocess {
public:
	WordNode invertIdx;//wordnode's head, first wordnode
	TwogramNode Kgramhead;//BTNode's head, first BTNode
	vector<pair<string, double> >* vectorSpace;//docID=array's index, docID直接作为数组下标

	Preprocess() {
		invertIdx = NULL;
		Kgramhead = NULL;
		vectorSpace = new vector<pair<string, double> >[MAX_DOCNUM];
	}
	~Preprocess() {
		//delete[]invertIdx;
		//delete[]vectorSpace;
	};

	void Run();
private:
	void createInvertIdx();
	bool inList(string word, int docID, int pos);//find word, add docID and return true. If docID already exists, return true
										//if can't find word, return false
	WordNode addWordNode(string word, int docID,int pos); //add new node to index list,  return tail pointer
	void getFileNames(string path, vector<pair<string, int> >& Doc);

	void readInvertIdx();

	void createVectorSpace();

	void createKgram();
	void InsertIdxtoKgram(WordNode& n);
	TwogramNode FindKgramNode(string word);
	void createKgramNode(WordNode& n, string word);
	bool KgramwordRepeat(WordNode& n, TwogramNode& kgram_n);
};


