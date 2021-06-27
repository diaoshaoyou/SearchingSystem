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

    //生成一个长度为0x500（合10进制数：1280）的cryptTable[0x500] 
    //initialize ctypt table  初始化加密表，后面用来生成hash值用的
    void prepareCryptTable();

    //plainTXT为要被哈希的明文，其中hashType 为hash的类型，  return plainTXT's hash value
    unsigned long BZHashVal(const char* plainTXT, unsigned long hashType);

    bool insertKey(char* key, WordNode newPWord);
       
    //target 为要在hash表中查找的字符串；lpTable 为存储字符串hash值的hash表；nTableSize 为hash表的长度：   
    int getTablePos(char* target);
};