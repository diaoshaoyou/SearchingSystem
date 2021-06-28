#include "WildMatch.h"
#include "string"
using namespace std;

void WildMatch::Run(vector<string>& inputList, TwogramNode KgramHash[]) {
	if (checkSyntax(inputList) == false) {//check basic syntax error
		resDoc.clear();
		resDoc.emplace_back(ERROR);
		return;
	}

	search(inputList, KgramHash);
}

bool WildMatch::checkSyntax(vector<string>& inputList) {
	int preWord = 0;
	int preStar = 0;
	int Star = 0;
	if (inputList.size() != 1)return false;
	for (string in : inputList)
		for (int i = 0; i < in.length(); i++) {
			if (in[i] == '*') {
				if (preStar)return false;
				preWord = 0;
				preStar = 1;
				Star++;
			}
			else {
				preWord = 1;
				preStar = 0;
			}
		}

	if (Star == 0)return false;
	return true;
}

void WildMatch::search(vector<string>& inputList, TwogramNode KgramHash[]) {
	string temp = "";
	KgramList.clear();
	resDoc.clear();
	resWord.clear();
	for (string input : inputList) {
		input.insert(0, "$");//Head add $
		input.append("$");//tail add $
		//cout << input << endl;
		for (int i = 0; i < input.length(); i++) {
			if (input[i] == '*') {//intercepts the word between *
				KgramList.push_back(temp);
				wordtokgram(temp, KgramHash);
				temp = "";
			}
			else temp.insert(temp.length(), 1, input[i]);
		}
		if (temp != "") {
			KgramList.push_back(temp);
			wordtokgram(temp, KgramHash);
			temp = "";
		}
	}
	CheckRes();
	if (!resWord.empty())ResWordtoResDoc();
	//delete duplicate ID and sort, È¥ÖØ+ÅÅÐò
	set<int>s(resDoc.begin(), resDoc.end());
	resDoc.assign(s.begin(), s.end());
	sort(resDoc.begin(), resDoc.end());
	if (resDoc.empty()) resDoc.emplace_back(EMPTY);
}

void WildMatch::wordtokgram(string word, TwogramNode KgramHash[]) {
	vector<node> temRes;
	/*cout << "--------wordtokgram---------" << endl;
	cout << word << endl;*/
	while (word.size() >= 2) {
		string temp;
		//cout << "temp = " << temp << endl;
		temp = word.substr(0, 2);
		word = word.substr(1);
		TwogramNode twogram_n = FindKgramNode(temp, KgramHash);
		if (twogram_n != NULL) {
			if (resWord.empty()) {//the first result save into resWord
				for (int i = 0; i < twogram_n->wordNum; i++) {
					/*cout << "empty!" << endl;
					cout << twogram_n->wordList[i] << endl;*/
					if (!CheckNodeExist(twogram_n->wordList[i]->WordVal, resWord))resWord.push_back(*twogram_n->wordList[i]);
				}
			}
			else {//the results are temporary temRes
				temRes.clear();
				for (int i = 0; i < twogram_n->wordNum; i++) {
					/*cout << "NO" << endl;
					cout << twogram_n->wordList[i] << endl;*/
					if (CheckNodeExist(twogram_n->wordList[i]->WordVal, resWord) && !CheckNodeExist(twogram_n->wordList[i]->WordVal, temRes))temRes.push_back(*twogram_n->wordList[i]); //merge with resWord and do not repeat in temRes
				}
				//resWord = temRes
				resWord.clear();
				resWord = temRes;
				//for (node n : temRes) resWord.push_back(n);

			}
			//debug
			/*for (node n : resWord)cout << n.WordVal << " ";
			cout << endl;
			cout <<endl<< temp << " : ";
			for(int i=0;i<twogram_n->wordNum;i++)cout<<twogram_n->wordList[i]->WordVal << " ";
			cout << endl;*/
		}
	}
}



TwogramNode WildMatch::FindKgramNode(string word, TwogramNode KgramHash[]) {
	return KgramHash[int(word[0]) * 255 + int(word[1])];
}

bool WildMatch::CheckNodeExist(string word, vector<node>& res) {
	if (res.empty())return false;
	for (node n : res) {
		if (word._Equal(n.WordVal))return true;
	}
	return false;
}

void WildMatch::CheckRes() {
	vector<node> temRes = resWord;
	int i = 0;
	int del = 0;
	//cout << "-----------checkres---------" << endl;
	for (node n : temRes) {
		string temp = n.WordVal;
		//cout << temp << endl;
		temp.insert(0, "$");
		temp.append("$");
		for (string s : KgramList) {
			//cout << temp <<endl;
			int first = temp.find(s);
			//cout << "first = " << first<<endl;
			if (first == -1) {//delete error result
				//cout << resWord.size() << " " << i << endl;
				resWord.erase(resWord.begin() + i);
				//del++;
				i--;
				break;
			}
			temp = temp.substr(first + s.length());
		}
		i++;
	}
	//cout << "---" << endl;
}

void WildMatch::ResWordtoResDoc() {
	cout << "RESULT:" << endl;
	for (node n : resWord) {
		cout << n.WordVal << " ";
		for (int i = 0; i < n.DocNum; i++)
			for (int ID : n.DocList[i]) {
				resDoc.push_back(ID);
				break;
			}
	}
	cout << endl;
}
