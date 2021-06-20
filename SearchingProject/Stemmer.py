# -*- coding: utf-8 -*-
"""
@author: Hanjing Zhou
"""
import nltk.stem as stem;
import nltk.tokenize as tk
from nltk.corpus import stopwords
import os

# =============================================================================
# stopwords.words("english")
# {‘ourselves’, ‘hers’, ‘between’, ‘yourself’, ‘but’, ‘again’, ‘there’, \
#  ‘about’, ‘once’, ‘during’, ‘out’, ‘very’, ‘having’, ‘with’, ‘they’, \
#  ‘own’, ‘an’, ‘be’, ‘some’, ‘for’, ‘do’, ‘its’, ‘yours’, ‘such’, \
#  ‘into’, ‘of’, ‘most’, ‘itself’, ‘other’, ‘off’, ‘is’, ‘s’, ‘am’, \
#  ‘or’, ‘who’, ‘as’, ‘from’, ‘him’, ‘each’, ‘the’, ‘themselves’, \
#  ‘until’, ‘below’, ‘are’, ‘we’, ‘these’, ‘your’, ‘his’, ‘through’, \
#  ‘don’, ‘nor’, ‘me’, ‘were’, ‘her’, ‘more’, ‘himself’, ‘this’, \
#  ‘down’, ‘should’, ‘our’, ‘their’, ‘while’, ‘above’, ‘both’, ‘up’, \
#  ‘to’, ‘ours’, ‘had’, ‘she’, ‘all’, ‘no’, ‘when’, ‘at’, ‘any’, \
#  ‘before’, ‘them’, ‘same’, ‘and’, ‘been’, ‘have’, ‘in’, ‘will’, \
#  ‘on’, ‘does’, ‘yourselves’, ‘then’, ‘that’, ‘because’, ‘what’, \
#  ‘over’, ‘why’, ‘so’, ‘can’, ‘did’, ‘not’, ‘now’, ‘under’, ‘he’, \
#  ‘you’, ‘herself’, ‘has’, ‘just’, ‘where’, ‘too’, ‘only’, ‘myself’, \
#  ‘which’, ‘those’, ‘i’, ‘after’, ‘few’, ‘whom’, ‘t’, ‘being’, ‘if’, \
#  ‘theirs’, ‘my’, ‘against’, ‘a’, ‘by’, ‘doing’, ‘it’, ‘how’, \
#  ‘further’, ‘was’, ‘here’, ‘than’}                
# =============================================================================

def Lemmatize(tokenList):
    lem = stem.WordNetLemmatizer()
    ret=[]
    for word in tokenList:
        word = lem.lemmatize(word, pos='n')# nouns to singulative 
        word = lem.lemmatize(word, pos='v') # verbs to original
        ret.append(word)
    return ret

def WriteData(result, file):
    path=os.path.join("DataBase",file)
    fout=open(path,"w")
    for w in result:
        fout.write(w+' ')
    fout.close()

def main():
    stops=stopwords.words("english")
    punctuation=set({',', '.', '+', '-', '*', '/', ';', '.', '#', \
                     '@', '$', '\'', '(', ')', '[', ']', '{', '}', \
                     '=', '|', '\\', ':', '?', '!', '~', '&', '^', \
                     '\"', '>', '<', '_', '`'})
    res=[]
    for root, dirs, files in os.walk("Reuters"):
        for file in files:
            res.clear()
            path=os.path.join(root,file)
            fin=open(path,"r")
            content=fin.read()
            fin.close()
            tokens = tk.WordPunctTokenizer().tokenize(content)
#           delete stop words and punctuations:
            for w in tokens:
                w=w.lower()# to lower case
                if w not in stops and w not in punctuation:
                    res.append(w)
#           lemmatize        
            res=Lemmatize(res) 
            WriteData(res, file)
            print(file+" done!")
        print("All files done!")
            

if __name__=='__main__':
    main()