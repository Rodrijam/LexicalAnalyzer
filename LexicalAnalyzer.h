/*
	Authors: James Rodriguez
    Project: 1
	File: LexicalAnalyzer.h
	Date: 10/8/19
*/

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>

using namespace std;

enum token_type {NONE = -10, ERROR_T, PREDICATE_T,
									EOF_T = 50, IDENT_T, NUMLIT_T, STRLIT_T, LISTOP_T,
									PLUS_T, MINUS_T, DIV_T, MULT_T, EQUALTO_T,
									GT_T, LT_T, GTE_T, LTE_T, LPAREN_T,
									RPAREN_T, SQUOTE_T, CONS_T, IF_T, COND_T, ELSE_T,
									DISPLAY_T, NEWLINE_T, AND_T, OR_T, NOT_T,
									DEFINE_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T,
									STRINGP_T, MODULO_T, ROUND_T};
									
class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type GetToken ();
	string GetTokenName (token_type t) const;
	string GetLexeme () const;
	void ReportError (const string & msg);
    private:
	ifstream input;
	ofstream listingFile;
	ofstream tokenFile;
	ofstream debugFile;
	token_type token;
	string strLine;
	int linenum;
	int pos;
	string lexeme;
	int errors;
    bool eof;
};
	
#endif
