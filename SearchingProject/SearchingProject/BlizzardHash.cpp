#pragma once
#include "BlizzardHash.h"
using namespace std;

    BlizzardHash::BlizzardHash() {
        freeNodeNum = 0;
        tableSize = MinTableSize;
        HashTable = NULL;
    };
    
    void BlizzardHash::CreateHashTable(WordNode invertIdx) {
        prepareCryptTable();
        string word;
        for (WordNode p = invertIdx; p != NULL; p = p->Next) {
            word = p->WordVal;
            if (insertKey((char*)word.c_str(), p) == false) {
                cout << MemErr << endl;
            }
        }
        //debug: cout << tableSize << endl;
    }
    WordNode BlizzardHash::GetWordNode(string word) {//can't find, return NULL
        int pos = getTablePos((char*)word.c_str());
        //if (pos < MinTableSize)
        //cout << word << ": " << pos << endl;
        if (pos == -1)
            return NULL;//not in database
        else
            return HashTable[pos].PWord;
    }

    //生成一个长度为0x500（合10进制数：1280）的cryptTable[0x500] 
    //initialize ctypt table  初始化加密表，后面用来生成hash值用的
    void BlizzardHash::prepareCryptTable() {
        unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;
        for (index1 = 0; index1 < 0x100; index1++) {
            for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100) {
                unsigned long temp1, temp2;
                seed = (seed * 125 + 3) % 0x2AAAAB;
                temp1 = (seed & 0xFFFF) << 0x10;
                seed = (seed * 125 + 3) % 0x2AAAAB;
                temp2 = (seed & 0xFFFF);
                cryptTable[index2] = (temp1 | temp2);
            }
        }
    }

    //plainTXT为要被哈希的明文，其中hashType 为hash的类型，  return plainTXT's hash value
    unsigned long BlizzardHash::BZHashVal(const char* plainTXT, unsigned long hashType) {
        unsigned char* key = (unsigned char*)plainTXT;
        unsigned long seed1 = 0x7FED7FED;
        unsigned long seed2 = 0xEEEEEEEE;
        int ch;

        while (*key != 0)
        {
            ch = *key++;
            seed1 = cryptTable[(hashType << 8) + ch] ^ (seed1 + seed2);
            seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
        }
        return seed1;
    }

    bool BlizzardHash::insertKey(char* key, WordNode newPWord) {
        if (freeNodeNum <= 0) {//hash table is full
            int oldLen = tableSize;
            freeNodeNum = oldLen;
            tableSize <<= 1;//size doubles
            HashTable = (Hashnode*)realloc(HashTable, tableSize * sizeof(HashTable[0]));
            if (HashTable == NULL) {
                return false;
            }
            else
                memset(&HashTable[oldLen], 0, freeNodeNum * sizeof(HashTable[0]));
        }
        unsigned int hash1 = BZHashVal(key, 0);
        unsigned int hash2 = BZHashVal(key, 1);
        unsigned int hash3 = BZHashVal(key, 2);
        unsigned int start = hash1 & (tableSize - 1);
        unsigned int pos = start;
        Hashnode* p = NULL;

        do {
            p = &HashTable[pos];
            if (!p->Hash1) {//hash1 is NULL, then this position is empty
                p->Hash1 = hash1;
                p->Hash2 = hash2;
                p->Hash3 = hash3;
                p->PWord = newPWord;
                freeNodeNum--;
                return true;
            }
            pos = (pos + 1) & (tableSize - 1);
        } while (start != pos);
        return false;//can't find any free space
    }

    //target 为要在hash表中查找的字符串；lpTable 为存储字符串hash值的hash表；nTableSize 为hash表的长度：   
    int BlizzardHash::getTablePos(char* target) {
        unsigned int hash1 = BZHashVal(target, 0);
        unsigned int hash2 = BZHashVal(target, 1);
        unsigned int hash3 = BZHashVal(target, 2);
        unsigned int start = hash1 & (tableSize - 1);
        unsigned int pos = start;
        int len = tableSize;

        //折半查找
        while (len >= MinTableSize) {
            pos = hash1 & (len - 1);
            if (HashTable[pos].Hash1 == hash1 && HashTable[pos].Hash2 == hash2 && HashTable[pos].Hash3 == hash3) {
                return pos;//获取哈希值对应的节点的下标
            }
            len >>= 1;
        }
        //折半查找不到则从hash1位置(hash1)开始遍历整个表（顺延获取）
        start = pos = hash1 & (tableSize - 1);//表长度必须是2的次方
        do {
            if (HashTable[pos].Hash1 == hash1 && HashTable[pos].Hash2 == hash2 && HashTable[pos].Hash3 == hash3) {
                return pos;//获取哈希值对应的节点的下标
            }
            pos = (pos + 1) & (tableSize - 1);//表长度必须是2的次方
        } while (start != pos);
        return -1;//can't find position
    }
