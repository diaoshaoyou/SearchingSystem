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

    //����һ������Ϊ0x500����10��������1280����cryptTable[0x500] 
    //initialize ctypt table  ��ʼ�����ܱ�������������hashֵ�õ�
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

    //plainTXTΪҪ����ϣ�����ģ�����hashType Ϊhash�����ͣ�  return plainTXT's hash value
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

    //target ΪҪ��hash���в��ҵ��ַ�����lpTable Ϊ�洢�ַ���hashֵ��hash��nTableSize Ϊhash��ĳ��ȣ�   
    int BlizzardHash::getTablePos(char* target) {
        unsigned int hash1 = BZHashVal(target, 0);
        unsigned int hash2 = BZHashVal(target, 1);
        unsigned int hash3 = BZHashVal(target, 2);
        unsigned int start = hash1 & (tableSize - 1);
        unsigned int pos = start;
        int len = tableSize;

        //�۰����
        while (len >= MinTableSize) {
            pos = hash1 & (len - 1);
            if (HashTable[pos].Hash1 == hash1 && HashTable[pos].Hash2 == hash2 && HashTable[pos].Hash3 == hash3) {
                return pos;//��ȡ��ϣֵ��Ӧ�Ľڵ���±�
            }
            len >>= 1;
        }
        //�۰���Ҳ������hash1λ��(hash1)��ʼ����������˳�ӻ�ȡ��
        start = pos = hash1 & (tableSize - 1);//���ȱ�����2�Ĵη�
        do {
            if (HashTable[pos].Hash1 == hash1 && HashTable[pos].Hash2 == hash2 && HashTable[pos].Hash3 == hash3) {
                return pos;//��ȡ��ϣֵ��Ӧ�Ľڵ���±�
            }
            pos = (pos + 1) & (tableSize - 1);//���ȱ�����2�Ĵη�
        } while (start != pos);
        return -1;//can't find position
    }
