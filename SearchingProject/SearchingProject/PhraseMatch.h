#pragma once
#pragma once
#define MAX_PHRASE_LEN 10
#include "global.h"
#include"BoolMatch.h"
class PhraseMatch {
public:
	vector<int> resDoc;

	PhraseMatch() {};
	~PhraseMatch() {};
	void Run(vector<string>& inputList, BlizzardHash& BZhash);
	//words must in database & must in same doc & must in order
private:
	//bool findOneWord(WordNode ptr, vector<string>& inputList, WordNode pwords[], int n);
	bool inSameDoc(WordNode pwords[], vector<int> sameDocIdx[], int n);
	bool inOrder(vector<int> posLists[], int n);
};