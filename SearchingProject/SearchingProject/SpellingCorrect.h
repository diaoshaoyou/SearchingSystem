#pragma once
#define ERROR -9999
#define EMPTY -99
#include "Preprocess.h"
#include <set>
#include <algorithm>
#define mx 1000
enum {

};

struct SpellingNode {
	struct node* word;
	int frequency;
}; 
typedef SpellingNode* SpellingList;

class SpellingCorrect {
public:
	vector<node> resWord;//result word

	SpellingCorrect() { };
	~SpellingCorrect() {};
	void Run(vector<string>& inputList, TwogramNode Kgramhead);//start bool search

private:
	SpellingList spellingList[mx];
	int mindistance;
	int spellingListNum;

	bool checkSyntax(vector<string>& inputList);//check basic syntax error

	void search(vector<string>& inputList, TwogramNode Kgramhead);//search database using post expression
	void wordtokgram(string word, TwogramNode Kgramhead);//turn word into 2-gram 
	TwogramNode FindKgramNode(string word, TwogramNode Kgramhead);

	int FindSpellingList(string word);
	int EditDistance(string input1, string input2);
	void SaveTop(int mxf);//only save max frequency
};