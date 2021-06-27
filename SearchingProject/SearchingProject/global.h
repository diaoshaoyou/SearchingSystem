#pragma once
#define MemErr "Error: no enough memory space!"
#define InstErr "#Error: invalid instruction!"
#define OverflowErr "#Error: instruction is too long!"
#define PHRASE_OVERFLOW -999
#define ERROR -9999
#define EMPTY -99
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
struct node {
	string WordVal;
	vector<vector<int> > DocList;//vecotr[0]=docID, vector[i]=word's position in doc
	int DocNum;
	struct node* Next;
};
typedef node* WordNode;