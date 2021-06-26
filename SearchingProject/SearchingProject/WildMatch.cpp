#include "WildMatch.h"
#include "string"
using namespace std;


void WildMatch::Run(vector<string>& inputList, TwogramNode Kgramhead) {
	if (checkSyntax(inputList) == false) {//check basic syntax error
		resDoc.clear();
		resDoc.emplace_back(ERROR);
		return;
	}

	search(inputList,Kgramhead);
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

void WildMatch::search(vector<string>& inputList, TwogramNode Kgramhead) {
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
				wordtokgram(temp,Kgramhead);
				temp = "";
			}
			else temp.insert(temp.length(),1,input[i]);
		}
		if (temp != "") {
			KgramList.push_back(temp);
			wordtokgram(temp,Kgramhead);
			temp = "";
		}
	}
	CheckRes();
	ResWordtoResDoc();
	if (resDoc.empty())resDoc.push_back(EMPTY);
}

void WildMatch::wordtokgram(string word, TwogramNode Kgramhead) {
	vector<node> temRes;
	while (word.size() >= 2) {
		string temp;
		temp = word.substr(0, 2);
		word = word.substr(1);
		TwogramNode twogram_n = FindKgramNode(temp,Kgramhead);
		if (twogram_n != NULL) {
			if (resWord.empty()) {//the first result save into resWord
				for (int i = 0; i < twogram_n->wordNum; i++) {
					if (!CheckNodeExist(twogram_n->wordList[i]->WordVal, resWord))resWord.push_back(*twogram_n->wordList[i]);
				}
			}
			else {//the results are temporary temRes
				temRes.clear();
				for (int i = 0; i < twogram_n->wordNum; i++) {
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
			/*cout <<endl<< temp << " : ";
			for(int i=0;i<twogram_n->wordNum;i++)cout<<twogram_n->wordList[i]->WordVal << " ";
			cout << endl;*/
		}
	}
}

TwogramNode WildMatch::FindKgramNode(string word, TwogramNode Kgramhead) {
	TwogramNode p = Kgramhead;
	while (p != NULL) {
		if (p->Kgramword._Equal(word))return p;
		p = p->Next;
	}
	return NULL;
}

bool WildMatch::CheckNodeExist(string word,vector<node>& res) {
	if (res.empty())return false;
	for (node n : res) {
		if (word._Equal(n.WordVal))return true;
	}
	return false;
}

void WildMatch::CheckRes() {
	vector<node> temRes = resWord;
	int i = 0;
	for (node n : temRes) {
		string temp = n.WordVal;
		temp.insert(0, "$");
		temp.append("$");
		for (string s : KgramList) {
			//cout << temp <<endl;
			int first = temp.find(s);
			//cout << "first = " << first<<endl;
			if (first== -1) {//delete error result
				DeleteWord(n.WordVal);
				break;
			}
			temp = temp.substr(first+s.length());
		}
		i++;
	}
	
}

void WildMatch::ResWordtoResDoc() {
	cout << "RESULT:"<<endl;
	for (node n : resWord) {
		cout << n.WordVal<<" ";
		for(int i = 0;i<n.DocNum;i++)
			for (int ID : n.DocList[i]) {
				if(!CheckIDExist(ID))resDoc.push_back(ID);
				break;
			}
	}
	cout << endl;
}

void WildMatch::DeleteWord(string word) {
	int i = 0;
	for (node n : resWord) {
		if (word._Equal(n.WordVal)) {
			resWord.erase(resWord.begin() + i);
			break;
		}
		i++;
	}
}

bool WildMatch::CheckIDExist(int ID) {
	if (resDoc.empty())return false;
	for (int i : resDoc)
		if (i == ID)return true;
	return false;
}
