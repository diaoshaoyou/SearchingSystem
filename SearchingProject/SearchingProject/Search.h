#pragma once
//#include "VSM.h"
#include "BoolMatch.h"
#include "PhraseMatch.h"
#include "WildMatch.h"
#include "SpellingCorrect.h"
#include "CompressIndex.h"
#include "SynExtend.h"
#include "TopK.h"
enum {
	Exit = 0,
	Bool_Match = 1,
	Wild_Match = 2,
	Correct = 3,
	TopK = 4,
	Phrase_Match = 5,
	Synonym = 6
};

class Search {
public:
	BoolMatch* boolmatch;
	PhraseMatch* phrasematch;
	WildMatch* wildmatch;
	SpellingCorrect* spellingcorrect;
	CompressIndex* comIdx;
	SynExtend* synextend;
	TOPK* topk;
	Search() {
		boolmatch = new BoolMatch;
		phrasematch = new PhraseMatch;
		wildmatch = new WildMatch;
		spellingcorrect = new SpellingCorrect;
		comIdx = new CompressIndex;
		synextend = new SynExtend;
		topk = new TOPK;
	}
	~Search() {};
};