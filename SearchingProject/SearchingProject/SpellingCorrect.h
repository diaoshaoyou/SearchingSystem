#pragma once
#include "Preprocess.h"
#include "global.h"

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
	void Run(vector<string>& inputList, TwogramNode KgramHash[]);//start bool search

private:
	vector<SpellingNode*> spellingList;

	bool checkSyntax(vector<string>& inputList);//check basic syntax error

	void search(vector<string>& inputList, TwogramNode KgramHash[]);//search database using post expression
	void wordtokgram(string word, TwogramNode KgramHash[]);//turn word into 2-gram 
	TwogramNode FindKgramNode(string word, TwogramNode KgramHash[]);
	int FindSpellingList(string word);
	int EditDistance(string input1, string input2);
};