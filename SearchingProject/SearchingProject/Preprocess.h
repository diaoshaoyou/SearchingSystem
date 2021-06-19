#pragma once
#define MAX_DOCNUM 21600
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

class Preprocess {
public:
	WordNode invertIdx;//wordnode's head, first wordnode
	vector<pair<string, double> >* vectorSpace;//docID=array's index, docID直接作为数组下标

	Preprocess() {
		invertIdx = NULL;
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
};


