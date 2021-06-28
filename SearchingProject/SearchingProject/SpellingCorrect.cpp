#include "SpellingCorrect.h"

using namespace std;

void SpellingCorrect::Run(vector<string>& inputList, TwogramNode KgramHash[]) {
	if (checkSyntax(inputList) == false) {//check basic syntax error
		resWord.clear();
		cout << "#Error: invalid instruction!" << endl;
		return;
	}
	search(inputList, KgramHash);
	if (resWord.empty()) cout << "EMPTY!!!" << endl;
	else {
		cout << "RESULT:" << endl;
		for (node n : resWord) {
			cout << n.WordVal << " ";
		}
		cout << endl;
	}
}

bool SpellingCorrect::checkSyntax(vector<string>& inputList) {
	int preLetter = 0;//a-z
	if (inputList.size() != 1)return false;
	for (string in : inputList) {
		for (int i = 0; i < in.length(); i++) {
			if (in[i] < 'a' && in[i] > 'z')return false;
		}
	}
	return true;
}

void SpellingCorrect::search(vector<string>& inputList, TwogramNode KgramHash[]) {
	resWord.clear();
	spellingList.clear();
	int mindistance = -1;
	for (string input : inputList) {
		input.insert(0, "$");//Head add $
		input.append("$");//tail add $
		//cout << input << endl;
		wordtokgram(input, KgramHash);
		if (spellingList.size() == 0)return;
		input = input.substr(1, input.size() - 2);
		for (int i = 0; i < spellingList.size(); i++) {//find min edit distance
			int distance = EditDistance(input, spellingList[i]->word->WordVal);
			//cout << spellingList[i]->word->WordVal << " " << distance<<endl;
			if (distance < mindistance || mindistance == -1) {
				resWord.clear();
				resWord.push_back(*spellingList[i]->word);
				mindistance = distance;
				//cout << spellingList[i]->word->WordVal << " min = " << mindistance << endl;
			}
			else if (distance == mindistance) resWord.push_back(*spellingList[i]->word);
		}
	}

	//debug
	/*cout << "-----------------search debug----------------" << endl;
	for (int i = 0; i < spellingList.size(); i++)cout << spellingList[i]->word->WordVal << " " << spellingList[i]->frequency << endl;*/
}

void SpellingCorrect::wordtokgram(string word, TwogramNode KgramHash[]) {
	int len = word.size() - 2;
	while (word.size() >= 2) {
		string temp;
		temp = word.substr(0, 2);
		word = word.substr(1);
		TwogramNode twogram_n = FindKgramNode(temp, KgramHash);
		if (twogram_n != NULL) {
			for (int i = 0; i < twogram_n->wordNum; i++)
				if (abs(int(twogram_n->wordList[i]->WordVal.size() - len)) < 2) {//The length difference between reserved and input word shall not exceed 1
					int find = FindSpellingList(twogram_n->wordList[i]->WordVal);
					if (find == -1) {
						SpellingNode* n = new SpellingNode;
						n->frequency = 1;
						n->word = twogram_n->wordList[i];
						spellingList.push_back(n);
					}
					else spellingList[find]->frequency++;
				}
		}
	}
	//debug
	//for (int i = 0; i < spellingListNum; i++)cout << spellingList[i]->word->WordVal << " " << spellingList[i]->frequency << endl;
}

TwogramNode SpellingCorrect::FindKgramNode(string word, TwogramNode KgramHash[]) {
	if (int(word[0]) < 0 || int(word[0]) > 255 || int(word[1]) < 0 || int(word[1]) > 255)return NULL;
	return KgramHash[int(word[0]) * 255 + int(word[1])];
}

int SpellingCorrect::FindSpellingList(string word) {
	if (spellingList.size() == 0)return -1;
	for (int i = 0; i < spellingList.size(); i++) {
		if (word._Equal(spellingList[i]->word->WordVal))return i;
	}
	return -1;
}

int SpellingCorrect::EditDistance(string input1, string input2) {
	int dp[20][20];
	for (int i = 0; i <= input1.size(); i++)
		for (int j = 0; j <= input2.size(); j++) {
			if (i == 0 && j == 0)dp[i][j] = 0;
			else if (i == 0)dp[i][j] = dp[i][j - 1] + 1;
			else if (j == 0)dp[i][j] = dp[i - 1][j] + 1;
			else {
				dp[i][j] = dp[i][j - 1] + 1;
				if (dp[i][j] > dp[i - 1][j] + 1)dp[i][j] = dp[i - 1][j] + 1;
				int dpval = dp[i - 1][j - 1];
				if (input1[i - 1] != input2[j - 1])dpval++;
				if (dp[i][j] > dpval)dp[i][j] = dpval;
			}
		}
	//debug
	/*cout << "--------EditDistance debug-------"<<endl;
	cout << input1 << " " << input2 << endl;
	for (int i = 0; i <= input1.size(); i++) {
		for (int j = 0; j <= input2.size(); j++) cout << dp[i][j] << " ";
			cout << endl;
	}*/
	return dp[input1.size()][input2.size()];
}
