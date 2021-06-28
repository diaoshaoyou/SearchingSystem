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
				temp = "";
			}
			else temp.insert(temp.length(), 1, input[i]);
		}
		if (temp != "") {
			KgramList.push_back(temp);
			temp = "";
		}

	}
	for (int i = 0; i < KgramList.size(); i++) {
		wordtokgram(KgramList[i], KgramHash);
	}

	//CheckRes();
	//if (!resWord.empty())ResWordtoResDoc();
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
			for (int i = 0; i < twogram_n->wordNum; i++) {
				string tempres = twogram_n->wordList[i]->WordVal;
				tempres.insert(0, "$");
				tempres.append("$");
				bool correct = true;
				for (string s : KgramList) {
					//cout << tempres <<endl;
					int first = tempres.find(s);
					//cout << "first = " << first<<endl;
					if (first == -1) {//delete error result
						correct = false;
						break;
					}
					tempres = tempres.substr(first + s.length());
				}
				if (correct) {
					resWord.push_back(*twogram_n->wordList[i]);
					for (int j = 0; j < twogram_n->wordList[i]->DocNum; j++)
						for (int ID : twogram_n->wordList[i]->DocList[j]) {
							resDoc.push_back(ID);
							break;
						}
				}
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
	if (int(word[0]) < 0 || int(word[0]) > 255 || int(word[1]) < 0 || int(word[1]) > 255)return NULL;
	return KgramHash[int(word[0]) * 255 + int(word[1])];
}
