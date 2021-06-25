#include "PhraseMatch.h"
void PhraseMatch::Run(vector<string>& inputList, WordNode invertIdx) {
	//words must in database & must in same doc & must in order
	resDoc.clear();
	int n = inputList.size();
	if (n == 1) {//same as boolmatch
		BoolMatch bm = BoolMatch();
		bm.Run(inputList, invertIdx);
		resDoc = bm.resDoc;
		return;
	}
	else if (n > 10) {
		resDoc.emplace_back(PHRASE_OVERFLOW);
		return;
	}
	WordNode pwords[MAX_PHRASE_LEN];
	vector<int> sameDocIdx[MAX_PHRASE_LEN];
	int find = 0;
	int none = 0;

	for (WordNode p = invertIdx; p != NULL; p = p->Next) {
		if (findOneWord(p, inputList, pwords, n)) {
			find++;
		}
	}
	if (find == n) {//input words are all in database
		if (!inSameDoc(pwords, sameDocIdx, n)) {//input words are not in same doc
			none = 1;
		}
	}
	else {
		none = 1;
	}
	if (none) {//no doc satisfied
		resDoc.clear();
		resDoc.emplace_back(EMPTY);
	}
}
bool PhraseMatch::findOneWord(WordNode ptr, vector<string>& inputList, WordNode pwords[], int n) {
	for (int i = 0; i < n; i++) {
		if (ptr->WordVal._Equal(inputList[i])) {
			pwords[i] = ptr;
			return true;
		}
	}
	return false;
}
bool PhraseMatch::inSameDoc(WordNode pwords[], vector<int> sameDocIdx[], int n) {
	resDoc.clear();
	set<int> sameDocID;
	int i = 0, j = 0;
	while (i < pwords[0]->DocNum && j < pwords[1]->DocNum) {
		//for debug: cout << pwords[0]->DocList[i][0] << "  " << pwords[1]->DocList[j][0] << endl;
		if (pwords[0]->DocList[i][0] < pwords[1]->DocList[j][0]) {
			i++;
		}
		else if (pwords[0]->DocList[i][0] > pwords[1]->DocList[j][0]) {
			j++;
		}
		else {//==
			sameDocID.insert(pwords[0]->DocList[i][0]);
			i++;
			j++;
		}
	}
	set<int> tmp;
	for (int k = 2; k < n; k++) {
		for (int t = 0; t < pwords[k]->DocNum; t++) {
			if (sameDocID.count(pwords[k]->DocList[t][0])) {//same as former set
				tmp.insert(pwords[k]->DocList[t][0]);
			}
		}
		sameDocID = tmp;
		tmp.clear();
	}
	if (sameDocID.empty()) return false;
	else {
		vector<int> posLists[MAX_PHRASE_LEN];
		int tmpNum = 0;
		set<int>::iterator iter = sameDocID.begin();
		while (iter != sameDocID.end()) {//every same doc
			for (int k = 0; k < n; k++) {
				for (int t = 0; t < pwords[k]->DocNum; t++) {
					if (pwords[k]->DocList[t][0] == (*iter)) {
						posLists[tmpNum].assign(pwords[k]->DocList[t].begin() + 1, pwords[k]->DocList[t].end());
						tmpNum++;
						break;
					}
				}
			}
			if (inOrder(posLists, n)) {//not in order
				resDoc.emplace_back(*iter);
			}
			iter++;
		}
		if (resDoc.empty()) return false;//in same doc & but not in order
		else return true;
	}
}
bool PhraseMatch::inOrder(vector<int> posLists[], int n) {
	int startPos = 0;
	int order = 1;
	for (int i = 0; i < posLists[0].size(); i++) {
		order = 1;
		startPos = posLists[0][i];
		for (int j = 1; j < n; j++) {
			for (int k = 0; k < posLists[j].size(); k++) {
				if (startPos + j == posLists[j][k]) {
					order++;
					break;
				}
			}

		}
		if (order == n) {
			return true;
		}
	}
	return false;
}