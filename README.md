# SearchingSystem

## 环境

Windows10

VS2019  debug模式，x86

## 文件结构

#### Stemmer.py

对Reuters语料库进行预处理的python源代码，包括去除停用词、去除标点、词形还原等。预处理后的文件存在DataBase文件夹中。

#### DataBase

用nltk库提前对Reuters进行预处理后的语料文件。预处理后的文件都是小写，所以查询时先把用户输入转化成小写，再检索。

#### InvertedIndex.txt

存倒排索引。若把它删了，能重新读取DataBase创建索引；否则直接读取它到内存。

第一行：总文件数。

第二行：所有文件ID和对应总词数，空格分隔。比如 1 30 5 20表示1号文件有30词，5号文件有20词

后面：单词+文件ID+单词在该文件的位置。文件之间用|分隔，每个单词之间用#分隔

比如  Argentine
1 10 | 5 5 | 6 5 #            Argentine出现在1号文件第10个位置，5号文件第5个位置，6号文件第5个位置

#### Query.txt

输入一个查询方式+查询语句。为了方便调试，输入用读文件，main中注释掉while循环。

## 功能

bool match：布尔查询

wild match：通配符查询

spelling correct：拼写校正

topK match：topK查询

phrase match：短语查询

synonym match：同义词查询

老师要求的内容似乎不止这些，再加。。。

## 倒排索引

Preprocess.h和Preprocess.cpp里

```c++
struct node{
	string WordVal;
	vector<vector<int> > DocList;//vecotr[0]=docID, vector[i]=word's position in doc
	int DocNum;
	struct node* Next;
};
typedef node* WordNode;
WordNode invertIdx;//wordnode's head, first wordnode
```

```mermaid
graph LR
0[node]-->1[node]-->2[node]-->3[node]
```

invertIdx：指向第一个node

每个word都由一个node存储。其中

DocNum：该word出现的文件总数

DocList：实则是个二维数组，每个元素vector<int>存文件ID和所在位置，文件ID存在vector[0]。比如：

| 文件ID | 所在位置 |
| ------ | -------- |
| 1      | 1,2      |
| 5      | 3,10     |
| 6      | 4        |



## 向量空间模型

Preprocess.h和Preprocess.cpp里

```c++
vector<pair<string, double> >* vectorSpace;//docID直接作为数组下标
```

每个文件都对应一个向量，向量中的每个值都是pair<word, wf-idf>

不存，每次运行后都算出来。



## K-gram索引

Preprocess.h和Preprocess.cpp里

```c++
typedef struct KgramNode {
	string Kgramword;
	struct node* wordList[mx];//There are wordNum words in the kgram
	int wordNum;//total num
	struct KgramNode* Next;
};
typedef KgramNode* TwogramNode;
TwogramNode Kgramhead;//KgramNode's head, first KgramNode
```

本次使用了2-gram索引，每个2-gram词有自己的字符数组wordList[]直接链接struct node。



## 索引压缩

CompressIndex.h里

```c++
struct node_com {
	int FirstDoc;//第一个doc ID
	int DocNum_com;
	vector<char> OtherDoc;//其他ID与前一个的差值
	vector<int>* Pos;//words' positions in every doc
};
typedef node_com* WordNode_com;
vector<string> WordVal;//all characters in database
WordNode_com* compressIdx;
```

单词的压缩：把所有单词都存在一个vector<string>里

docID的压缩：每个单词中，存第一个ID、该ID与前一个的差值、对应doc中单词的位置

差值：用$\gamma$编码，存在char里，最多7位，最大可表示的差值为15

单词位置未进行压缩

## 布尔查询

#### 输入

操作符有：OR、AND、NOT（全大写）

输入样例：

```c
1
COCOA OR shower AND NOT nnn
```



## 同义词扩展

#### 输入

在Query.txt输入命令+需要扩展的单词

输入样例：

```c
6
bank
```

#### 运行

先运行``GetSyn.py``获取同义词集合，存在``Synonym.txt``中。再运行C++代码进行查询/输出同义词(组)