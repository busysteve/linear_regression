// rowparser.h

#ifndef _ROWPARSER_H
#define _ROWPARSER_H

#include "parser.h"
#include <string>
#include <vector>
#include <map>

class RowParserException : public ParserException {
public:
	RowParserException( const char * szMsg )
		: ParserException( szMsg )
	{}
};


class RowParser : public Parser {
private:
	char	*m_szLine;
	bool GetNextLine();
	void Init();

protected:
	int		m_iRowCounter;
	int		m_iFirstRowItemCount;
	int		m_iLastRowItemCount;
	int		m_iMinimumRowItemCount;
	int		m_iMaximumRowItemCount;
	char	m_szItemDelimiter[256];
	char	*m_bufItemDelimiter;
	int		m_ItemDelimiterLen;
	char	m_szRowDelimiter[256];
	char	*m_bufRowDelimiter;
	int		m_RowDelimiterLen;
	bool	m_bStringsQuoted;
	bool	m_bUseHeaders;
	bool	m_bHeadersLoaded;
	bool	m_bUseStartMarker;
    bool    m_bIncStartMarker;
    bool    m_bStartMarkerFound;
    bool    m_bParsePairs;

	std::vector<std::string>		m_vecRow;
	std::map< std::string, int >	m_mapHeaders;
	std::map< int, std::string >	m_mapHeaderLookup;

    std::string m_strStartMarker;

	RowParser();
	void SetItemDelimiter   ( const char * szItemDelimiter );
	void SetRowDelimiter    ( const char * szRowDelimiter );
    void SetStartMarker     ( const char * szStartMarker ) {m_strStartMarker = szStartMarker;m_bUseStartMarker = true;return;};
    void SetIncStartMarker  ( bool b ) { m_bIncStartMarker = b;return;};
	void SetStringsQuoted   ( bool bQuoted );
	void SetUseHeaders      ( bool bHeaders );

public:
	RowParser(FILE* fpFile,
		const char* szItemDelimiter,
		const char* szRowDelimiter,
		bool bQuoted,
		bool bUseHeaders);

	RowParser(FILE* fpFile,
		const char* szItemDelimiter,
		const char* szRowDelimiter,
		bool bQuoted,
		bool bUseHeaders,
		const char* szStartMarker,
		bool bIncStartmarker);

	RowParser(const char* szFilename,
		const char* szItemDelimiter,
		const char* szRowDelimiter,
		bool bQuoted,
		bool bUseHeaders);

	RowParser(const char* szFilename,
		const char* szItemDelimiter,
		const char* szRowDelimiter,
		bool bQuoted,
		bool bUseHeaders,
		const char* szStartMarker,
		bool bIncStartmarker);

	RowParser( const char * pBuffer, unsigned int uiSize,
				const char * szItemDelimiter, 
				const char * szRowDelimiter, 
				bool bQuoted,
				bool bUseHeaders );

	RowParser( const char * pBuffer, unsigned int uiSize,
				const char * szItemDelimiter, 
				const char * szRowDelimiter, 
				bool bQuoted,
				bool bUseHeaders,
                const char * szStartMarker,
                bool bIncStartmarker );

	~RowParser();

	int GetItemCount();
	const char * GetItem( int iIndex ) const;
        const char * GetHeader( int iIndex );
	int GetItemValue( int iIndex );


	bool ParseNextRow();
    void ParsePairs() { m_bParsePairs = true;return;};
	void Reset();

	// Statistical functions
	int GetNumRowsProcessed();
	int GetFirstRowItemCount();
	int GetLastRowItemCount();
	int GetMinimumRowItemCount();
	int GetMaximumRowItemCount();

	// Operator overloads
    std::string operator[]( int iIndex );
    std::string operator[]( const char* szHeader );
};

#endif // _ROWPARSER_H


