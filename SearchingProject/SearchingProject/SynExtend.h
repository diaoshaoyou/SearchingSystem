#pragma once
#include "global.h"
#include "PhraseMatch.h"
using namespace std;

class SynExtend {
public:
	SynExtend() {};
	~SynExtend() {};
	void Run(vector<string>& inputList, BlizzardHash& BZhash) {
		if (inputList.size() > 1) {
			cout << OverflowErr << endl;
			return;
		}

		findSyn();
		cout << "\"" << inputList[0] << "\" synonyms: " << endl;
		if (Synonyms.empty()) {
			cout << "none" << endl;
			return;
		}
		int n = 0;
		PhraseMatch pm = PhraseMatch();
		for (auto input : Synonyms) {
			pm.Run(input, BZhash);
			if (pm.resDoc[0] == EMPTY || pm.resDoc[0] == PHRASE_OVERFLOW) {
			}
			else {
				for (string str : input) {
					cout << str << " ";
				}
				cout << endl;
				for (int i : pm.resDoc) {
					cout << "doc " << i << " ";
				}
				cout << endl;
			}
		}
		//printSyn();
	}
private:
	vector<vector<string> > Synonyms;

	/*void printSyn() {
		for (auto syn : Synonyms) {
			for (int i = 0; i < syn.size(); i++) {
				cout << syn[i];
				if (i < syn.size() - 1)
					cout << " ";
			}
			cout << endl;
		}
	}*/

	void findSyn() {
		//python code: find synonyms with nltk
		vector<string> synList;//from python code
		string str;
		fstream fin("../Synonym.txt", fstream::in);
		while (fin >> str) {
			synList.emplace_back(str);
		}
		fin.close();
		Synonyms.clear();
		if (synList.empty()) {
			return;
		}
		set<string> s(synList.begin(), synList.end());//ȥ??
		synList.assign(s.begin(), s.end());
		//for debug:
		/*for (string str : synList) {
			cout << str << endl;
		}*/
		int start = 0;
		int end = 0;
		
		vector<string> tmpV;
		for (int i = 0; i < synList.size(); i++) {
			tmpV.clear();
			start = 0;
			while (1) {
				end = synList[i].find("_", start);
				if (end < 0) {
					break;
				}
				tmpV.emplace_back(synList[i].substr(start, end - start));
				//for debug: cout << synList[i].substr(start, end - start) << " ";
				start = end + 1;
			}
			//for debug: cout << synList[i].substr(start, synList[i].length() - start) << endl;
			tmpV.emplace_back(synList[i].substr(start, synList[i].length() - start));
			Synonyms.emplace_back(tmpV);
		}
	}
	
};