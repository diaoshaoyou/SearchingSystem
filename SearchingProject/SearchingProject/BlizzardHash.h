#pragma once
#define MinTableSize 16 //2^4
#include "global.h"
using namespace std;

//    string WordVal;
//    vector<vector<int> > DocList;//vecotr[0]=docID, vector[i]=word's position in doc
//    int DocNum;
//    struct node* Next;
//};
//typedef node* WordNode;

struct Hashnode{
    unsigned int Hash1;
    unsigned int Hash2;
    unsigned int Hash3;
    WordNode PWord;
};

class BlizzardHash {
public:
   
    BlizzardHash();
	~BlizzardHash() {};
    void CreateHashTable(WordNode invertIdx);

    WordNode GetWordNode(string word);//can't find, return NULL
       
private:
    unsigned long cryptTable[0x500];
    int tableSize;//must be 2^n
    int freeNodeNum;//free node amount  
    Hashnode* HashTable;

    //����һ������Ϊ0x500����10��������1280����cryptTable[0x500] 
    //initialize ctypt table  ��ʼ�����ܱ�������������hashֵ�õ�
    void prepareCryptTable();

    //plainTXTΪҪ����ϣ�����ģ�����hashType Ϊhash�����ͣ�  return plainTXT's hash value
    unsigned long BZHashVal(const char* plainTXT, unsigned long hashType);

    bool insertKey(char* key, WordNode newPWord);
       
    //target ΪҪ��hash���в��ҵ��ַ�����lpTable Ϊ�洢�ַ���hashֵ��hash��nTableSize Ϊhash��ĳ��ȣ�   
    int getTablePos(char* target);
};