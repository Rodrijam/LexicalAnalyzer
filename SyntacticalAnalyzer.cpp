#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	int len = strlen(filename);
	if (filename[len-3] != '.' && filename[len-2] != 's' && filename[len-1] != 's')
	{
		cerr << filename << "is not a '.ss' file. Exiting..." << endl;
		exit(0);		
	}
	string name = filename;
	name = name.substr(0, len-3);

	lex = new LexicalAnalyzer (filename);
	token_type t;
	ofstream outFile;
	outFile.open(name + ".p1", fstream::app);
	
	while ((t = lex->GetToken()) != EOF_T)
	{
        if (t != -10)
        {
            outFile << left;
            outFile << setw(16);
            outFile << lex->GetTokenName(t);
            outFile << lex->GetLexeme() << endl;
        }
	}
    outFile << lex->GetTokenName(t);
	outFile.close();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}
