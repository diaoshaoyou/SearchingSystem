#pragma once
#define MAX_DOCNUM 21600
#include <stdio.h>
#include "global.h"
using namespace std;
extern int TotalDoc;//total doc number
extern int* docLen;//word len of every doc
extern int TotalWord;//total word number
extern vector<int> docList;//all docID
struct node_com {
	int FirstDoc;//first doc ID of the word
	int DocNum_com;
	vector<char> OtherDoc;//docs' ID in offset pattern
	vector<int>* Pos;//words' positions in every doc
};
typedef node_com* WordNode_com;

class CompressIndex {
public:
	vector<string> WordVal;//all characters in database
	WordNode_com* compressIdx;

	CompressIndex() { 
		compressIdx = NULL;
		docLen = new int[MAX_DOCNUM];
	};
	~CompressIndex() {};

	void Invert2CompressIdx() {
		fstream in("../InvertedIndex.txt", fstream::in);
		string str;
		string str1;
		int off;//ID offset
		int formerID = 0;
		int nextWord = 1;
		int firstDoc = 1;
		int firstPos = 1;

		in >> str;
		TotalDoc = atoi(str.c_str());
		in >> str;
		TotalWord = atoi(str.c_str());
		//compressIdx = (WordNode_com*)malloc(TotalWord * sizeof(WordNode_com));
		compressIdx = new WordNode_com[TotalWord];
		/*for (int i = 0; i < TotalWord; i++) {
			compressIdx[i]->DocNum_com = 0;
			compressIdx[i]->FirstDoc = -1;
			compressIdx[i]->Pos = new vector<int>[MAX_DOCNUM];
		}*/

		for (int i = 0; i < TotalDoc; i++) {
			in >> str;
			in >> str1;
			docList.emplace_back(atoi(str.c_str()));
			docLen[docList.back()] = atoi(str1.c_str());
		}
		int i = -1;
		while (in >> str) {
			if (nextWord) {//word
				WordVal.emplace_back(str);
				nextWord = 0;
				i++;
				continue;
			}
			else {//docID
				if (firstDoc && firstPos) {
					formerID = atoi(str.c_str());
					WordNode_com New = new node_com;

					New->FirstDoc = formerID;
					New->Pos = new vector<int>[MAX_DOCNUM];
					New->DocNum_com = 1;
					compressIdx[i] = New;
					firstPos = 0;
				}
				else if (str._Equal("|")) {
					if (firstDoc) firstDoc = 0;
					firstPos = 1;
				}
				else if (str._Equal("#")) {//one doc ends
					nextWord = 1;
					firstDoc = 1;
					firstPos = 1;
				}
				else if (firstDoc && !firstPos) {
					compressIdx[i]->Pos[compressIdx[i]->DocNum_com - 1].emplace_back(atoi(str.c_str()));
				}
				else if (!firstDoc && firstPos) {
					off = atoi(str.c_str()) - formerID;
					compressIdx[i]->OtherDoc.emplace_back(rCode(off));
					compressIdx[i]->DocNum_com++;
					firstPos = 0;
				}
				else {//!firstDoc && !firstPos
					compressIdx[i]->Pos[compressIdx[i]->DocNum_com - 1].emplace_back(atoi(str.c_str()));
					
				}
			}
		}
		in.close();
		//for debug: print compressIndex
		cout << "docNum=" << TotalDoc << endl;
		for (int i = 0; i<TotalWord; i++) {
			cout << WordVal[i] << endl;
			cout << compressIdx[i]->FirstDoc << " ";	
			for (int j : compressIdx[i]->Pos[0]) {
					cout << j << " ";
			}
			if (compressIdx[i]->DocNum_com > 1)
				cout << "| ";
			for (int k = 0; k < compressIdx[i]->OtherDoc.size(); k++) {
				printf("%d ", compressIdx[i]->OtherDoc[k]);
				for (int j : compressIdx[i]->Pos[k+1]) {
					cout << j << " ";
				}
				cout << "| ";
			}
			cout << "#" << endl;
		}
	}
private:
	char rCode(int num) {//convert number to binary rCode, max off=15
		vector<int> invertBinary;
		char res = 0;
		int len = 0;
		while (num) {
			invertBinary.emplace_back(num & 1);
			num = num >> 1;
		}
		invertBinary.pop_back();
		len = invertBinary.size();
		invertBinary.emplace_back(0);
		for (int i = 0; i < len; i++) {
			invertBinary.emplace_back(1);
		}
		for (int i = invertBinary.size() -1 ; i >= 0; i--) {
			//for debug: cout << invertBinary[i];
			if (invertBinary[i])//==1
				res |= 1;
			if (i == 0) break;
			res = res << 1;
			//for debug: printf("%d\n", res);
		}
		return res;
	}
};