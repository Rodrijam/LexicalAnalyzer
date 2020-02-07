/*
	Authors: James Rodriguez
    Project: 1
	File: LexicalAnalyzer.cpp
	Date: 10/8/19
*/

#include <iomanip>
#include <cstdlib>
#include <map>
#include <stdio.h>
#include <string.h>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = {	"EOF_T", "IDENT_T", "NUMLIT_T", "STRLIT_T", "LISTOP_T", "PLUS_T", 
								"MINUS_T", "DIV_T", "MULT_T", "EQUALTO_T", "GT_T",
								"LT_T", "GTE_T", "LTE_T", "LPAREN_T", "RPAREN_T", 
								"SQUOTE_T", "CONS_T", "IF_T", "COND_T", "ELSE_T",
								"DISPLAY_T", "NEWLINE_T", "AND_T", "OR_T", "NOT_T", "DEFINE_T",
								"NUMBERP_T", "LISTP_T", "ZEROP_T", "NULLP_T", "STRINGP_T",
								"MODULO_T", "ROUND_T"};

static map<char, int> lexMap = {
						{'+', -1}, {'-', -2}, {'*', -5}, {'.', -6}, {'(', -7},
						{')', -8}, {'>', -9}, {'\'', -10}, {'<', -11}, {'=', -12},
						{'_', -13}, {'?', -14}, {'/', -15}, {'"', -16},
						{'c', -18}, {'a', -19}, {'d', -20}, {'r', -21},
						};

static map<string,token_type> PredMap = {
						{"string?", STRINGP_T}, {"number?", NUMBERP_T}, {"list?", LISTP_T}, {"zero?", ZEROP_T}, {"null?", NULLP_T}
						};					

						
static map<string, token_type> KWmap = {
						{"cons",CONS_T}, {"if",IF_T}, {"cond",COND_T}, {"else",ELSE_T}, {"display",DISPLAY_T},
						{"newline",NEWLINE_T}, {"and",AND_T}, {"or",OR_T}, {"not",NOT_T}, {"define",DEFINE_T},
                        {"modulo",MODULO_T}, {"round",ROUND_T}
						};
								
int dataList[26][25] = 
	{{-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11, -11, -11},
	{1,2,15,3,18,13,23,9,10,5,14,7,11,-11,-11,12,17,-11,20,18,18,18,-11,-11,-11},
	{2,-11,-11,3,-11,-11,25,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,55},
	{3,-11,-11,3,-11,-11,4,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,52},
	{4,-11,-11,4,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,52},
	{5,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,6,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,60},
	{6,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,62},
	{7,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,8,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,61},
	{8,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,63},
	{9,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,64},
	{10,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,65},
	{11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,59},
	{12,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,57},
	{13,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,58},
	{14,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,66},
	{15,-11,-11,3,-11,-11,25,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,56},
	{16,-11,-11,19,18,-11,-11,-11,-11,-11,-11,-11,-11,19,-11,-11,-11,-11,18,18,16,24,-11,-11,51},
	{17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,53,17,17,17,17,17,17,17,53},
	{18,-11,-11,19,18,-11,-11,-11,-11,-11,-11,-11,-11,19,-12,-11,-11,-11,18,18,18,18,-11,-11,51},
	{19,-11,-11,19,19,-11,-11,-11,-11,-11,-11,-11,-11,19,-11,-11,-11,-11,19,19,19,19,-11,-11,51},
	{20,-11,-11,19,18,-11,-11,-11,-11,-11,-11,-11,-11,19,-11,-11,-11,-11,18,21,16,18,-11,-11,51},
	{21,-11,-11,19,18,-11,-11,-11,-11,-11,-11,-11,-11,19,-11,-11,-11,-11,18,18,21,22,-11,-11,51},
	{22,-11,-11,19,18,-11,-11,-11,-11,-11,-11,-11,-11,19,-11,-11,-11,-11,18,18,18,18,-11,-11,54},
	{23,-11,-11,4,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11},
	{24,-11,-11,19,18,-11,-11,-11,-11,-11,-11,-11,-11,19,-11,-11,-11,-11,18,18,18,18,-11,-11,54},
	{25,-11,-11,3,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11}
	};


LexicalAnalyzer::LexicalAnalyzer (char * filename)
{	
    //initialize and open the debug and listing files
	int len = strlen(filename);
	
	if (filename[len-3] != '.' && filename[len-2] != 's' && filename[len-1] != 's')
	{
		cerr << filename << "is not a '.ss' file. Exiting..." << endl;
		exit(0);		
	}
	
	cout << "Input file: " << filename << endl;
	string name = filename;
	name = name.substr(0, len-3);
	token = NONE;
	
    debugFile.open(name + ".dbg");
    listingFile.open(name + ".lst");
	tokenFile.open(name + ".p1");
    listingFile << "Input file: " << filename << endl;
    
   
	ifstream dataFile("DFAset.csv");
	
    eof = false;

	input.open(filename);
	errors = 0;
	linenum = 0;
	pos = 0;
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
	cout << errors << " errors found in input file" << endl;
	input.close();
    debugFile.close();
    listingFile.close();
	tokenFile.close();
}

token_type LexicalAnalyzer::GetToken ()
{
	// This function will find the next lexeme in the input file and return
	// the token_type value associated with that lexeme
	
	token = NONE;

	if (eof)
	{
		token = EOF_T;
		listingFile << '\t' << linenum+1 << ": " << endl;
        listingFile << errors << " errors found in input file" << endl;
		debugFile << "End of File" << endl;
		return token;
	}		
	
	if (pos == 0)
	{
		getline(input,strLine);
		listingFile << '\t' << linenum+1 << ": " << strLine << endl;
		debugFile << "Line #" << linenum+1  << ": " << strLine << endl;
		linenum++;
	}
	
	if (strLine.empty())
	{
		debugFile << "Empty Line" << endl;
		if (input.eof())
		{
			token = EOF_T;
			listingFile << '\t' << linenum+1 << ": " << endl;
			listingFile << errors << " errors found in input file" << endl;
		}
		else
			token = this->GetToken ();
		
		if (token == -11)
			errors++;
		return token;
	}
	
	char inChar;
	int col = 23;
	int state = 1;
	lexeme = "";
	
	for (int i = pos; i < strLine.size(); i++) // iterate through the given line
	{
		inChar = strLine[i]; // get ith character in the line
		
		debugFile << "Line Number: " << linenum << " Character: " << inChar << endl;
		
		// check type of inChar
		if (isdigit(inChar))
		{
			col = 3;
		}
		else if (isalpha(inChar)) //letter check
		{
			if (lexMap.count(inChar) == 0)
				col = 4;
			else
				col = (lexMap.find(inChar)->second * -1);				
		}
		else if (isspace(inChar)) //whitespace check
		{
			col = 22;
		}
		else
		{
			if (lexMap.count(inChar) == 0)
				col = 23;
			else
				col = (lexMap.find(inChar)->second * -1);
		}
		
		int newState = dataList[state][col];	 // set new state based on type's column
		
		debugFile << "New State for " << inChar << " is " << newState << " from column: " << col << " at previous state " << state << endl;
		
		if (newState == -11 && state == 1 && col == 22)
			{
			pos++;
			token = this->GetToken ();	
			if (token == -11)
			{	
				string errorMsg = "Error at " + to_string(linenum) + "," + to_string(pos -1) + ": Invalid character found: ";
				if (lexeme == "+." || lexeme == "-.")
					errorMsg += lexeme;
				else
					errorMsg += inChar;
				ReportError(errorMsg);
				errors++;
			}	
			return token;
			}		
		
		
		if (newState != -11 || state == 1)					// if newState is not an error add character to lexeme
			lexeme += inChar;
		if (newState == -12)								// lexeme is a predicate
		{
            if (PredMap.count(lexeme) == 0)
            {
                lexeme.erase(lexeme.size()-1);
				if (KWmap.count(lexeme) == 0)				// check if false predicate is a keyword
					token = IDENT_T;
				else
					token = KWmap.find(lexeme)->second;
				debugFile << "False predicate at " << lexeme << endl;
			}
            else
			{
				token = PredMap.find(lexeme)->second;
				debugFile << "Token predicate at " << lexeme << ": " << newState << " " << token_names[token - 50] << endl;
			}
			pos = pos + lexeme.size();
			return token;
		}	
		else if (newState == -11 || newState > 50)			// lexeme is at a final state (not a predicate)
		{
			int tokenState = dataList[state][24];
			debugFile << "Lexeme complete: " << lexeme << " at state " << tokenState << endl;
			token_type token = (token_type)tokenState; 
			if (KWmap.count(lexeme) == 0)
				token_type token = (token_type)tokenState;	// gets token name based on tokenState
			else
				token = KWmap.find(lexeme)->second;
			pos = pos + lexeme.size();
			if (inChar == ' ')
				pos++;
			if (token == -11)
			{	
				string errorMsg = "Error at " + to_string(linenum) + "," + to_string(pos -1) + ": Invalid character found: ";
				if (lexeme == "+." || lexeme == "-.")
					errorMsg += lexeme;
				else
					errorMsg += inChar;
				ReportError(errorMsg);
				errors++;
				debugFile << "Token error at " << lexeme << ": " << newState << " " << token_names[token*0] << endl;
			}	
			return token;
		}	
		state = newState;
	}
	
	
	debugFile << "End of Line" << endl;

	
	if (pos == strLine.size()) // check for situational end of line
	{
		if (input.eof())
		{
			token = EOF_T;
			listingFile << '\t' << linenum+1 << ": " << endl;
			listingFile << errors << " errors found in input file" << endl;		
		}
		else
		{
			pos = 0;
			token = this->GetToken ();
		}
		if (token == -11)
		{	
			string errorMsg = "Error at " + to_string(linenum) + "," + to_string(pos -1) + ": Invalid character found: ";
			if (lexeme == "+." || lexeme == "-.")
				errorMsg += lexeme;
			else
				errorMsg += inChar;
			ReportError(errorMsg);
			errors++;
		}	
		return token;
	}			
	else if (input.eof() && state < 50) // returns token at the end of the file
	{
		state = dataList[state][24];
		debugFile << "Last Lexeme complete: " << lexeme << " at state " << state << endl;
		token_type token = (token_type)state;
		state = 1;
		eof = true;
		if (token == -11)
		{	
			string errorMsg = "Error at " + to_string(linenum) + "," + to_string(pos -1) + ": Invalid character found: ";
			if (lexeme == "+." || lexeme == "-.")
				errorMsg += lexeme;
			else
				errorMsg += inChar;
			ReportError(errorMsg);
			errors++;
		}	
		return token;
	}	
	else
	{
		int tokenState = dataList[state][24];
		debugFile << "Lexeme complete: " << lexeme << " at state " << tokenState << endl;
		token_type token = (token_type)tokenState; // gets token name based on tokenState
		if (KWmap.count(lexeme) == 0)
			token_type token = (token_type)tokenState;	// gets token name based on tokenState
		else
			token = KWmap.find(lexeme)->second;
		pos = 0;
		debugFile << "New Position: " << pos << endl;
		if (token == -11)
		{	
			string errorMsg = "Error at " + to_string(linenum) + "," + to_string(pos -1) + ": Invalid character found: ";
			if (lexeme == "+." || lexeme == "-.")
				errorMsg += lexeme;
			else
				errorMsg += inChar;
			ReportError(errorMsg);
			errors++;
		}	
		return token;
	}	
	return token;
		
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
	// The GetTokenName function returns a string containing the name of the
	// token passed to it. 
	
	if (t == -11)
		return "ERROR_T";
	return token_names[t-50];
}

string LexicalAnalyzer::GetLexeme () const
{
	// This function will return the lexeme found by the most recent call to 
	// the get_token function
	
	return lexeme;
}

void LexicalAnalyzer::ReportError (const string & msg)
{
	// This function will be called to write an error message to a file
    listingFile << msg << endl;
}
