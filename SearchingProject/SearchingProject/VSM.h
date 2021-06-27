#pragma once
#include "Preprocess.h"
#include "global.h"
extern int TotalDoc;//total doc number
extern int* docLen;
class VSM {
public:
	VSM() {};
	~VSM() {};
	double Tf_idf(WordNode idx, int docID, string word, int choice) {//choice=0, tf-idf; choice=1, wf-idf
		double idf = 0;
		double tf = 0;
		double wordNuminDoc = 0;
		double thisDocLen = 0;
		int flag = 0;
		if (docLen[docID] != -1) {
			thisDocLen = docLen[docID];
		}
		else{
			cout << "Error: document " << docID << "doesn't in database!" << endl;
			return 0;
		}
		for (WordNode p = idx; p != NULL; p = p->Next) {
			if (word._Equal(p->WordVal)) {//word exists in database
				for (int i = 0; i < p->DocNum; i++) {
					if (p->DocList[i].at(0) == docID) {
						wordNuminDoc = p->DocList[i].size() - 1;
						break;
					}
				}
				idf = log10f(TotalDoc/p->DocNum);
				break;
			}
			
		}
		tf = wordNuminDoc / thisDocLen;
		if (choice == 0 || tf == 0)
			return tf * idf;//tf-idf
		else
			return (1 + log10(tf)) * idf;//wf-idf
	}
	
};
