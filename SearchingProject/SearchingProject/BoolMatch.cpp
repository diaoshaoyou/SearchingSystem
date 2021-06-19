#include "BoolMatch.h"
using namespace std;

extern vector<int> docList;//all docID

void BoolMatch::Run(vector<string>& inputList, WordNode invertIdx) {
	if (checkSyntax(inputList) == false) {//check basic syntax error
		resDoc.clear();
		resDoc.emplace_back(ERROR);
		return;
	}
	mid2Post(inputList, invertIdx);
	search();
}

bool BoolMatch::checkSyntax(vector<string>& inputList) {
	int preNOT = 0;//former one is NOT
	int preWord = 0;//former one is word
	int preAND_OR = 0;
	for (string in : inputList) {
		if (in._Equal("NOT")) {
			if (preWord || preNOT)
				return false;
			preNOT = 1;
			preWord = 0;
			preAND_OR = 0;
		}
		else if (in._Equal("AND") || in._Equal("OR")) {
			if (preNOT || preAND_OR)
				return false;
			preAND_OR = 1;
			preNOT = 0;
			preWord = 0;
		}
		else {//word
			if (preWord) {
				return false;
			}
			preWord = 1;
			preNOT = 0;
			preAND_OR = 0;
		}
	}
	if (preNOT || preAND_OR)
		return false;//NOT is at the end, error
	return true;
}

void BoolMatch::mid2Post(vector<string>& inputList, WordNode invertIdx) {
	vector<string> opStack;
	postExp.clear();
	vector<int> wordVec, notVec, orVec, andVec;
	notVec.emplace_back(_NOT);
	orVec.emplace_back(_OR);
	andVec.emplace_back(_AND);

	for (string input : inputList) {
		if (input._Equal("OR")) {
			while (!opStack.empty() && !opStack.back()._Equal("OR")) {//"NOT"/"AND" on the top
				if (opStack.back()._Equal("AND")) {
					opStack.pop_back();//pop "AND"
					postExp.emplace_back(andVec);
				}
				else {//"NOT"
					opStack.pop_back();//pop "NOT"
					postExp.emplace_back(notVec);
				}
			}
			opStack.emplace_back("OR");
		}
		else if (input._Equal("AND")) {
			while (!opStack.empty() && opStack.back()._Equal("NOT")) {//"NOT" on the top
				opStack.pop_back();//pop "NOT"
				postExp.emplace_back(notVec);
			}
			opStack.emplace_back("AND");
		}
		else if (input._Equal("NOT")) {//push
			opStack.emplace_back("NOT");
		}
		else {//input==word
			getDoc(input, invertIdx, wordVec);
			postExp.emplace_back(wordVec);
		}
	}
	while (!opStack.empty()) {
		if (opStack.back()._Equal("NOT"))
			postExp.emplace_back(notVec);
		else if (opStack.back()._Equal("OR"))
			postExp.emplace_back(orVec);
		else if (opStack.back()._Equal("AND"))
			postExp.emplace_back(andVec);
		opStack.pop_back();
	}
	//cout << postExp.at(0).at(0) << endl << postExp.at(1).at(0) << endl << postExp.at(2).at(0);
}
void BoolMatch::getDoc(string& word, WordNode invertIdx, vector<int>& ret) {
	ret.clear();
	for (WordNode p = invertIdx; p != NULL; p = p->Next) {
		if (p->WordVal._Equal(word)) {
			for (int i = 0; i < p->DocNum; i++) {
				ret.emplace_back(p->DocList[i].at(0));
			}
			break;
		}
	}
	//if word not in database, docs is empty
	if (ret.empty())
		ret.emplace_back(EMPTY);
	else//not empty
		sort(ret.begin(), ret.end());

}
void BoolMatch::search() {

	vector<vector<int> > wStack;
	vector<int> operand1, operand2;
	vector<int> tmpRes;
	resDoc.clear();

	for (vector<int> docs : postExp) {
		if (docs.at(0) == _NOT) {//add to excludeDoc
			if (wStack.empty()) {
				resDoc.emplace_back(ERROR);
				return;
			}
			operand1 = wStack.back();
			wStack.pop_back();
			opNOT(operand1, tmpRes);
			wStack.emplace_back(tmpRes);
		}
		else if (docs.at(0) == _OR) {//add to resDoc 
			if (wStack.size() < 2) {
				resDoc.emplace_back(ERROR);
				return;
			}
			operand1 = wStack.back();
			wStack.pop_back();
			operand2 = wStack.back();
			wStack.pop_back();
			opOR_AND(operand1, operand2, 0, tmpRes);
			wStack.emplace_back(tmpRes);
		}
		else if (docs.at(0) == _AND) {
			if (wStack.size() < 2) {
				resDoc.emplace_back(ERROR);
				return;
			}
			operand1 = wStack.back();
			wStack.pop_back();
			operand2 = wStack.back();
			wStack.pop_back();
			opOR_AND(operand1, operand2, 1, tmpRes);
			wStack.emplace_back(tmpRes);
		}
		else {//word
			wStack.emplace_back(docs);
		}
	}

	if (wStack.size() > 1 || wStack.empty())
		resDoc.emplace_back(ERROR);//error
	else resDoc = wStack.back();
}

void BoolMatch::opOR_AND(vector<int>& docs1, vector<int>& docs2, int choice, vector<int>& tmpRes) {//choice=0, OR; choice=1, AND. result=tmpRes
	tmpRes.clear();
	//for debug:
	/*for (int i : docs1) {
		cout << i << " ";
	}
	cout << endl;
	for (int i : docs2) {
		cout << i << " ";
	}
	cout << endl;*/
	if (docs1.at(0) == EMPTY && docs2.at(0) == EMPTY) {
		tmpRes.emplace_back(EMPTY);
	}
	else if (docs1.at(0) == EMPTY && docs2.at(0) != EMPTY) {
		if (choice) {//AND
			tmpRes.emplace_back(EMPTY);
		}
		else {//OR
			tmpRes = docs2;
		}
	}
	else if (docs2.at(0) == EMPTY && docs1.at(0) != EMPTY) {
		if (choice) {//AND
			tmpRes.emplace_back(EMPTY);
		}
		else {//OR
			tmpRes = docs1;
		}
	}
	else {
		vector<int>::iterator itr1, itr2;
		for (itr1 = docs1.begin(), itr2 = docs2.begin(); itr1 != docs1.end() && itr2 != docs2.end();) {
			if ((*itr1) == (*itr2)) {
				tmpRes.emplace_back((*itr1));
				itr1++;
				itr2++;
			}
			else if ((*itr1) < (*itr2)) {
				if (choice == 0) {//OR
					tmpRes.emplace_back((*itr1));//push itr1
				}
				itr1++;
			}
			else if ((*itr1) > (*itr2)) {
				if (choice == 0) {//OR
					tmpRes.emplace_back((*itr2));//push itr2
				}
				itr2++;
			}
			//cout << tmpRes.back();
		}
		if (choice == 0) {//OR
			if (itr1 != docs1.end()) {
				for (; itr1 != docs1.end(); itr1++) {
					//cout << (*itr1);
					tmpRes.emplace_back((*itr1));
				}
			}
			if (itr2 != docs2.end()) {
				for (; itr2 != docs2.end(); itr2++) {
					tmpRes.emplace_back((*itr2));
				}
			}
		}
		//delete duplicate ID and sort, È¥ÖØ+ÅÅÐò
		set<int>s(tmpRes.begin(), tmpRes.end());
		tmpRes.assign(s.begin(), s.end());
		sort(tmpRes.begin(), tmpRes.end());
		if (tmpRes.empty()) tmpRes.emplace_back(EMPTY);
	}
}

void BoolMatch::opNOT(vector<int>& docs, vector<int>& tmpRes) {//docList - docs.  result=tmpRes
	tmpRes.clear();
	//for debug
	/*for (int i : docs) {
		cout << i << " ";
	}
	cout << endl;*/
	if (docs.at(0) == EMPTY) {//this word not in database
		tmpRes = docList;
	}
	else {
		set<int>s(docList.begin(), docList.end());
		for (int id : docs) {
			s.erase(id);
		}
		if (s.empty()) tmpRes.emplace_back(EMPTY);
		else tmpRes.assign(s.begin(), s.end());
	}
}