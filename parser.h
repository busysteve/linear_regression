// parser.h

#pragma	warning( disable : 4786 )

#include <stdio.h>
#include <string>

#ifndef _PARSER_H
#define _PARSER_H

class ParserException {
public:
	ParserException( const char * szMsg ) {
		strErrorMsg = szMsg;
	}

	std::string		strErrorMsg;
};

class Parser {
protected:
	FILE	    *m_fp;
	char	    *m_pBuffer;
	void	    (*m_pFunc)();
	bool	    m_bContinue;
	
	int      m_uiTotalBytes;
	int      m_uiBytesProcessed;

public:
	Parser();
	Parser( FILE* fpFile);
	Parser(const char* szFilename);
	Parser( char * pBuffer, unsigned int uiSize );
	virtual ~Parser();

	void ParseFile( FILE* fpFile);
	void ParseFile(const char* szFilename);
	void ParseBuffer( char * pBuffer, unsigned int uiSize );

	void Reset();


	// Callback utility functions
	void Pause();
	void Continue();
	void SetTotalBytes( unsigned int uiTotal );
	unsigned int GetTotalByteCount();
	unsigned int GetProcessedByteCount();
	int GetPercentComplete();
	void SetCallBack( void pFunc() );
};

#endif //_PARSER_H

