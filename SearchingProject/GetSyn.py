# -*- coding: utf-8 -*-
"""
Created on Sat Jun 26 09:23:08 2021

@author: Hanjing Zhou
"""
from nltk.corpus import wordnet

def main():
    fin=open("./Query.txt","r")
    word=fin.read()
    fin.close()
#    arr=query.split("\n")
#    word=arr[1]
    print(word)
    if(word=="" or word.find(' ') != -1):
        print("Error!")
        return
    fout=open("./Synonym.txt","w")
    for synSet in wordnet.synsets(word):
        for w in synSet.lemmas():
            fout.write(w.name().lower()+'\n')
    fout.close()
    print("done!")
    
if __name__=='__main__':
    main()