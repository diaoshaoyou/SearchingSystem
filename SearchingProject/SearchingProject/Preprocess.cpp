#include "Preprocess.h"

int TotalDoc;//total doc number
int* docLen;//word len of every doc
int TotalWord;//total word number
vector<int> docList;//all docID

void Preprocess::Run() {
	docLen = new int[MAX_DOCNUM];
	for (int i = 0; i < MAX_DOCNUM; i++) {
		docLen[i] = -1;
	}

	fstream f("../InvertedIndex.txt", fstream::in);
	bool flag = f.is_open();
	//debug: cout << flag << endl;
	f.close();
	if (flag) {
		readInvertIdx();
	}
	else
		createInvertIdx();
	createVectorSpace();
	createKgram();
	BZhash = BlizzardHash();
	BZhash.CreateHashTable(invertIdx);//set hash table
	
	cout << "Prework done!" << endl << endl;
}

void Preprocess::createInvertIdx() {
	string word = string("");
	int pos = 1;
	vector<pair<string, int > > Doc;
	clock_t startT, endT;
	startT = clock();
	getFileNames("../DataBase", Doc);
	TotalDoc = Doc.size();
	for (auto doc : Doc) {
		pos = 1;
		string filepath = "../DataBase/";
		filepath.append(doc.first);
		//for debug: cout << filepath;
		fstream fin(filepath, fstream::in);

		//generate index
		while (fin >> word) {
			//for debug: cout << word << endl;
			if (!inList(word, doc.second, pos)) {//word not in list
				addWordNode(word, doc.second, pos);
				TotalWord++;
			}
			pos++;
		}
		docLen[doc.second] = pos - 1;
		fin.close();
	}
	endT = clock();
	cout << "cost " << (double)(endT - startT) / CLOCKS_PER_SEC << "s ";

	//debug:
	//*int max = 0;
	//double sum = 0;
	//for (WordNode p = invertIdx; p != NULL; p = p->Next) {
	//	sum += p->DocNum;
	//	if (p->DocNum > max)
	//		max = p->DocNum;
	//}
	//cout << "max:" << max << " avg: " << sum / TotalWord << endl;*/

	//store index
	fstream fout("../InvertedIndex.txt", fstream::out);
	fout << TotalDoc << " " << TotalWord << "\n";//store total doc number and total word number
	//store word len of every doc
	for (int i = 0; i < MAX_DOCNUM; i++) {
		if (docLen[i] != -1)
			fout << i << " " << docLen[i] << " ";
	}
	fout << "\n";

	for (WordNode p = invertIdx; p != NULL; p = p->Next) {
		fout << p->WordVal << "\n";
		for (int i = 0; i < p->DocNum; i++) {
			for (int j : p->DocList[i]) {
				fout << j << " ";
			}
			if (i < p->DocNum - 1)
				fout << "| ";
		}
		fout << "#" << "\n";//end with #
	}
	fout.close();
}

bool Preprocess::inList(string word, int docID, int pos) {
	//find word, add docID and return true. If docID already exists, return true
	//if can't find word, return false
	if (invertIdx == NULL) return false;
	int flag = 0;
	for (WordNode p = invertIdx; p != NULL; p = p->Next) {
		if (word._Equal(p->WordVal)) {//word in wordList
			for (int i = 0; i < p->DocNum; i++) {
				if (p->DocList[i].at(0) == docID) {//docID in docList
					p->DocList[i].emplace_back(pos);//add new position
					flag = 1;
					break;
				}
			}
			if (flag == 0) {//docID not in docList
				addNewDoc(p, docID, pos);
			}
			return true;
		}
	}
	return false;
}
WordNode Preprocess::addWordNode(string word, int docID, int pos) {
//add new node to index list
//return tail pointer

	//assign new node
	WordNode newnode = new node;
	newnode->WordVal = word;
	vector<int> newDoc;
	newDoc.emplace_back(docID);
	if (pos != -1) newDoc.emplace_back(pos);
	newnode->DocList.clear();
	newnode->DocList.emplace_back(newDoc);
	newnode->Next = NULL;
	newnode->DocNum = 1;

	//add new node to inverted index
	if (invertIdx == NULL) {
		invertIdx = newnode;
	}
	else {
		WordNode p = invertIdx;
		while (p->Next != NULL) { p = p->Next; }
		p->Next = newnode;
	}
	return newnode;

}
void Preprocess::addNewDoc(WordNode p, int docID, int pos) {//add and sort docID
	int i = 0;
	for (i = p->DocNum - 1; i >= 0; i--) {
		if (docID < p->DocList[i][0]) {//move forward
			if (i == p->DocNum - 1) {
				p->DocList.emplace_back(p->DocList[i]);
			}
			else
				p->DocList[i + 1] = p->DocList[i];
		}
		else break;
	}
	if (i == p->DocNum - 1) {
		vector<int> tmp;
		tmp.emplace_back(docID);
		tmp.emplace_back(pos);
		p->DocList.emplace_back(tmp);
	}
	else {
		p->DocList[i + 1].clear();
		p->DocList[i + 1].emplace_back(docID);
		p->DocList[i + 1].emplace_back(pos);
	}
	p->DocNum++;
}
void Preprocess::readInvertIdx() {
	clock_t startT, endT;
	string str;
	string str1;
	string w;//wordValue
	int nextWord = 1;
	int firstDoc = 1;
	int firstPos = 1;
	WordNode wLoc = NULL;

	startT = clock();
	fstream in("../InvertedIndex.txt", fstream::in);
	in >> str;
	TotalDoc = atoi(str.c_str());
	in >> str;
	TotalWord = atoi(str.c_str());//total word

	for (int i = 0; i < TotalDoc; i++) {
		in >> str;
		in >> str1;
		docList.emplace_back(atoi(str.c_str()));
		docLen[docList.back()] = atoi(str1.c_str());
	}
	
	while (in >> str) {
		if (nextWord) {//word
			w = str;
			nextWord = 0;
			continue;
		}
		else {//docID
			if (firstDoc && firstPos) {
				wLoc = addWordNode(w, atoi(str.c_str()), -1);//-1 is temporary
				firstPos = 0;
			}
			else if (str._Equal("|")) {
				if (firstDoc) firstDoc = 0;
				firstPos = 1;
			}
			else if (str._Equal("#")) {//one doc ends
				//wLoc = addWordNode(w, atoi(str.c_str()), -1);//new word node
				nextWord = 1;
				firstDoc = 1;
				firstPos = 1;
			}
			else if (firstDoc && !firstPos) {
				wLoc->DocList[wLoc->DocNum - 1].emplace_back(atoi(str.c_str()));//add a new pos into first doc
			}
			else if (!firstDoc && firstPos) {
				vector<int> tmp;
				tmp.emplace_back(atoi(str.c_str()));
				wLoc->DocList.emplace_back(tmp);//add a new docID
				wLoc->DocNum++;
				firstPos = 0;
			}
			else {//!firstDoc && !firstPos
				wLoc->DocList[wLoc->DocNum - 1].emplace_back(atoi(str.c_str()));//add a new pos
			}
		}

	}
	in.close();
	endT = clock();
	cout << "cost " << (double)(endT - startT) / CLOCKS_PER_SEC << "s ";
	//for debug: print invertedIndex
	/*cout << "docNum=" << TotalDoc << endl;
	for (WordNode p = invertIdx; p != NULL; p = p->Next) {
		cout << p->WordVal << endl;
		for (int i = 0; i < p->DocNum; i++) {
			for (int j : p->DocList[i]) {
				cout << j << " ";
			}
			cout << "| ";
		}
		cout << endl;
	}*/
}

void Preprocess::getFileNames(string path, vector<pair<string, int> >& Doc)
{
	intptr_t hFile = 0;//file handle
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	string name;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			if (fileinfo.name[0] == '.')
				continue;
			name = fileinfo.name;
			docList.emplace_back(atoi(name.substr(0, name.rfind(".")).c_str()));
			//for debug: cout << name << endl;
			Doc.emplace_back(name, docList.back());
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void Preprocess::createVectorSpace() {//use wf-idf
	double idf = 0;
	double tf = 0;
	double wf = 0;
	int wordNuminDoc = 0;
	int docID = 0;

	for (WordNode p = invertIdx; p != NULL; p = p->Next) {
		idf = log10f(TotalDoc / p->DocNum);
		for (int i = 0; i < p->DocNum; i++) {//every doc
			docID = p->DocList[i].at(0);
			wordNuminDoc = p->DocList[i].size() - 1;
			tf =1.0 * wordNuminDoc / docLen[docID] ;
			wf = (tf == 0) ? 0 : 1 + log10(tf);
			vectorSpace[docID].emplace_back(p->WordVal, wf * idf);			
			//for debug: cout << docID << p->WordVal << endl << wf*idf;	
		}
	}	
	//for debug: print
	/*for (int i = 0; i < MAX_DOCNUM; i++) {
		if (docLen[i] != -1) {
			cout << i << endl;
			for (auto p : vectorSpace[i]) {
				cout << p.first << " " << p.second << "#";
			}
			cout << endl;
		}
	}*/
}
void Preprocess::createKgram() {
	//create Kgram
	WordNode p = invertIdx;
	while (p != NULL) {
		InsertIdxtoKgram(p);
		p = p->Next;
	}

	//debug
	/*TwogramNode temp = Kgramhead;
	while (temp != NULL) {
		std::cout << temp->Kgramword<<":";
		for (int i = 0; i < temp->wordNum; i++) std::cout << temp->wordList[i]->WordVal << " ";
		std::cout << endl;
		temp = temp->Next;
	}*/
}

void Preprocess::InsertIdxtoKgram(WordNode& n) {
	string word = n->WordVal;
	word.insert(0, "$");
	word.append("$");
	//std::cout << word << endl;
	while (!word._Equal("$")) {
		string temp;
		temp = word.substr(0, 2);
		word = word.substr(1);
		TwogramNode twogram_n = FindKgramNode(temp);
		if (twogram_n != NULL) {
			if (KgramwordRepeat(n, twogram_n)) {
				twogram_n->wordList[twogram_n->wordNum] = n;
				twogram_n->wordNum++;
			}
		}
		else createKgramNode(n, temp);

	}

}

TwogramNode Preprocess::FindKgramNode(string word) {
	TwogramNode p = Kgramhead;
	while (p != NULL) {
		if (p->Kgramword._Equal(word))return p;
		p = p->Next;
	}
	return NULL;
}

void Preprocess::createKgramNode(WordNode& n, string word) {
	//std::cout << word << endl;
	TwogramNode kgram_n = new KgramNode;
	kgram_n->Kgramword = word;
	kgram_n->wordNum = 1;
	kgram_n->wordList[0] = n;
	kgram_n->Next = Kgramhead;
	Kgramhead = kgram_n;
}

bool Preprocess::KgramwordRepeat(WordNode& n, TwogramNode& kgram_n) {
	for (int i = 0; i < kgram_n->wordNum; i++)
		if (n->WordVal._Equal(kgram_n->wordList[i]->WordVal))return false;
	return true;
}