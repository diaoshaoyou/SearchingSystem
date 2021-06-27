#include "TopK.h"
#include <string>
using namespace std;


void TOPK::Run(vector<string>& inputList, vector<pair<string, double> >* vectorspace) {
	if (checkSyntax(inputList, vectorspace) == false) {//check basic syntax error
		resDoc.clear();
		resDoc.emplace_back(ERROR);
		return;
	}
	search( vectorspace);
	if (resDoc.empty())resDoc.push_back(EMPTY);
}

bool TOPK::checkSyntax(vector<string>& inputList, vector<pair<string, double> >* vectorspace) {
	k = 0;
	inputID = 0;
	if (inputList.size() != 2)return false;
	string s = inputList[0];
	for (int i = 0; i < s.size(); i++) {
		if (s[i] < '0' || s[i]>'9')return false;
	}
	k = atoi(s.c_str());
	s = inputList[1];
	if (s.size() <= 3)return false;
	string temp = s.substr(0, 3);
	if (!temp._Equal("doc"))return false;
	s = s.substr(3);
	for (int i = 0; i < s.size(); i++) {
		if (s[i] < '0' || s[i]>'9')return false;
	}
	inputID = atoi(s.c_str());
	if (vectorspace[inputID].empty())return false;
	return true;
}

void TOPK::search( vector<pair<string, double> >* vectorspace) {
	resDoc.clear();
	MxHeapHead = NULL;
	/*cout << "---------TOPK debug----------" << endl;
	for(int i = 0;i< MAX_DOCNUM;i++)
		if(!vectorspace[i].empty()){
			for (auto p : vectorspace[i]) {
				cout << p.first << " " << p.second << "#";
			}
			cout << endl;
		}
	cout << k << endl;
	cout << inputID << endl;*/
	double inputIDdiv = 0;
	for (auto p : vectorspace[inputID]) {
		inputIDdiv += (p.second * p.second);
	}
	inputIDdiv = sqrt(inputIDdiv);
	double score = 0;
	for (int i = 0; i < MAX_DOCNUM; i++) {
		if (i != inputID && !vectorspace[i].empty()) {
			score = 0;
			double div = inputIDdiv;
			double IDdiv = 0;
			for (auto p : vectorspace[i]) {
				double cos = p.second * FindTFIDF(vectorspace[inputID],p.first);
				score += cos;
				IDdiv += (p.second * p.second);
			}
			div = IDdiv * inputIDdiv;
			score = score / div;
			cout << "doc" << i << " " << score << endl;
			InsertHeap(i,score);
		}
	}
	for (int i = 0; i < k; i++) {
		int docID = PopHeap();
		if (docID != -1)resDoc.push_back(docID);
		else return;
	}
}

double TOPK::FindTFIDF(vector<pair<string, double>>vector, string word) {
	for (auto p : vector) {
		if (word._Equal(p.first))return p.second;
	}
	return 0;
}

void TOPK::InsertHeap(int docID,double score) {
	ScoreNode* p = MxHeapHead;
	int docID_temp;
	double score_temp;
	if (MxHeapHead == NULL) {
		MxHeapHead = createScoreNode(docID, score);
		return;
	}

	while (p != NULL) {
		//cout << p->docID <<" "<<p->score<< endl;
		if (p->score >= score) {
			if (p->LeftNode == NULL) {
				p->LeftNode = createScoreNode(docID, score);
				return;
			}
			else if (p->RightNode == NULL) {
				p->RightNode = createScoreNode(docID, score);
				return;
			}
			else p = p->LeftNode;
		}
		else {
			docID_temp = p->docID;
			score_temp = p->score;
			p->docID = docID;
			p->score = score;
			docID = docID_temp;
			score = score_temp;
			p = MxHeapHead;
		}
	}

}

ScoreNode* TOPK::createScoreNode(int docID, double score) {
	ScoreNode* new_node = new ScoreNode;
	new_node->docID = docID;
	new_node->score = score;
	new_node->LeftNode = NULL;
	new_node->RightNode = NULL;
	return new_node;
}

int TOPK::PopHeap() {
	int docID = -1;
	if (MxHeapHead != NULL)docID = MxHeapHead->docID;
	else return -1;
	ScoreNode* p = MxHeapHead;
	ScoreNode* parent = NULL;
	int subtree = 0;//Left = 1 of Right = 1 subtree
	//cout << "------PopHeap debug------" << endl;
	while (p != NULL) {
		//cout << p->docID << endl;
		if (p->LeftNode != NULL && p->RightNode != NULL) {
			if (p->LeftNode->score > p->RightNode->score) {
				p->docID = p->LeftNode->docID;
				p->score = p->LeftNode->score;
				parent = p;
				p = p->LeftNode;
				subtree = 1;
			}
			else {
				p->docID = p->RightNode->docID;
				p->score = p->RightNode->score;
				parent = p;
				p = p->RightNode;
				subtree = -1;
			}
		}
		else if(p->LeftNode == NULL && p->RightNode == NULL){
			if (parent == NULL)MxHeapHead = NULL;
			if (subtree == 1)parent->LeftNode = NULL;
			if (subtree == -1) parent->RightNode = NULL;
			p = NULL;
		}
		else {
			if (p->LeftNode == NULL) {
				p->docID = p->RightNode->docID;
				p->score = p->RightNode->score;
				parent = p;
				p = p->RightNode;
				subtree = -1;
			}
			else if (p->RightNode == NULL) {
				p->docID = p->LeftNode->docID;
				p->score = p->LeftNode->score;
				parent = p;
				p = p->LeftNode;
				subtree = 1;
			}
		}
	}
	return docID;
}