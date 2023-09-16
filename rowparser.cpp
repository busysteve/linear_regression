#include "rowparser.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

RowParser::RowParser() {
	Init();
}

RowParser::RowParser(FILE* fpFile,
	const char* szItemDelimiter,
	const char* szRowDelimiter,
	bool bQuoted,
	bool bUseHeaders) {
	Init();

	Parser::ParseFile(fpFile);
	SetItemDelimiter(szItemDelimiter);
	SetRowDelimiter(szRowDelimiter);
	SetStringsQuoted(bQuoted);
	SetUseHeaders(bUseHeaders);
}

RowParser::RowParser(const char* szFilename,
	const char* szItemDelimiter,
	const char* szRowDelimiter,
	bool bQuoted,
	bool bUseHeaders) {
	Init();

	Parser::ParseFile(szFilename);
	SetItemDelimiter(szItemDelimiter);
	SetRowDelimiter(szRowDelimiter);
	SetStringsQuoted(bQuoted);
	SetUseHeaders(bUseHeaders);
}

RowParser::RowParser(const char* szFilename,
	const char* szItemDelimiter,
	const char* szRowDelimiter,
	bool bQuoted,
	bool bUseHeaders,
	const char* szStartMarker,
	bool bIncStartMarker) {
	Init();

	Parser::ParseFile(szFilename);
	SetItemDelimiter(szItemDelimiter);
	SetRowDelimiter(szRowDelimiter);
	SetStringsQuoted(bQuoted);
	SetUseHeaders(bUseHeaders);
	SetStartMarker(szStartMarker);
	SetIncStartMarker(bIncStartMarker);
}

RowParser::RowParser(FILE* fpFile,
	const char* szItemDelimiter,
	const char* szRowDelimiter,
	bool bQuoted,
	bool bUseHeaders,
	const char* szStartMarker,
	bool bIncStartMarker) {
	Init();

	Parser::ParseFile(fpFile);
	SetItemDelimiter(szItemDelimiter);
	SetRowDelimiter(szRowDelimiter);
	SetStringsQuoted(bQuoted);
	SetUseHeaders(bUseHeaders);
	SetStartMarker(szStartMarker);
	SetIncStartMarker(bIncStartMarker);
}

RowParser::RowParser( const char * pBuffer, unsigned int uiSize,
			const char * szItemDelimiter, 
			const char * szRowDelimiter, 
			bool bQuoted,
			bool bUseHeaders ) {
	Init();

	Parser::ParseBuffer( const_cast<char*>(pBuffer), uiSize );
	SetItemDelimiter( szItemDelimiter );
	SetRowDelimiter( szRowDelimiter );
	SetStringsQuoted( bQuoted );
	SetUseHeaders( bUseHeaders );
}

RowParser::RowParser( const char * pBuffer, unsigned int uiSize, 
			const char * szItemDelimiter, 
			const char * szRowDelimiter, 
			bool bQuoted,
			bool bUseHeaders,
            const char * szStartMarker,
            bool bIncStartMarker ) {
	Init();

	Parser::ParseBuffer( const_cast<char*>(pBuffer), uiSize );
	SetItemDelimiter( szItemDelimiter );
	SetRowDelimiter( szRowDelimiter );
	SetStringsQuoted( bQuoted );
	SetUseHeaders( bUseHeaders );
    SetStartMarker( szStartMarker );
    SetIncStartMarker(bIncStartMarker);
}

RowParser::~RowParser() {
	free( m_szLine );
	free( m_bufItemDelimiter );
	free( m_bufRowDelimiter );
}

void RowParser::Init() {
	m_szLine = (char*)malloc( 4096 * sizeof(char) );
	//this->m_szLine = new char[4096];

	m_bHeadersLoaded        = false;
	m_bUseHeaders           = false;
	m_bStringsQuoted        = false;
    m_bStartMarkerFound     = false;
    m_bUseStartMarker       = false;
    m_bIncStartMarker       = true;
	m_iRowCounter           = 0;
	m_iFirstRowItemCount    = 0;
	m_iLastRowItemCount     = 0;
	m_iMinimumRowItemCount  = 0;
	m_iMaximumRowItemCount  = 0;
	memset( m_szItemDelimiter, 0, sizeof( m_szItemDelimiter ) );
	memset( m_szRowDelimiter,  0, sizeof( m_szRowDelimiter  ) );
    m_strStartMarker = "";
}

void RowParser::SetItemDelimiter( const char * szItemDelimiter ) {
	strcpy( m_szItemDelimiter, szItemDelimiter );

	m_bufItemDelimiter = (char*)malloc( strlen( m_szItemDelimiter )+1 );	
	m_ItemDelimiterLen = strlen( m_szItemDelimiter );

}

void RowParser::SetRowDelimiter( const char * szRowDelimiter ) {
	strcpy( m_szRowDelimiter, szRowDelimiter );

	m_bufRowDelimiter = (char*)malloc( strlen( m_szRowDelimiter )+1 );	
	m_RowDelimiterLen = strlen( m_szRowDelimiter );
}

void RowParser::SetStringsQuoted( bool bQuoted ) {
	m_bStringsQuoted = bQuoted;
}


void RowParser::SetUseHeaders( bool bHeaders ) {
	m_bUseHeaders = bHeaders;
}

int RowParser::GetItemCount() {
	return m_vecRow.size();
}

const char * RowParser::GetItem( int iIndex ) const {
	return m_vecRow[iIndex].c_str();
}

const char * RowParser::GetHeader( int iIndex ) {
	return m_mapHeaderLookup[iIndex].c_str();
}

int RowParser::GetItemValue( int iIndex ) {
	return atoi( m_vecRow[iIndex].c_str() );
}

bool RowParser::GetNextLine( ) {
	//char	a;
	bool	bBailout = false;
	int		d;
	int		i=0;


	if( Parser::m_fp ) 	{
		for( i=0; feof(Parser::m_fp) == 0 ;  ) {		
			for( d=0; d < m_RowDelimiterLen; d++ ) {
				if( (m_bufRowDelimiter[d] = fgetc(Parser::m_fp)) == EOF ) {
					bBailout = true;
					break;
				}

				if( m_szRowDelimiter[d] == m_bufRowDelimiter[d] )
					bBailout = true;
				else {
					bBailout = false;
					d++;
					break;
				}
			}
			if( bBailout == true )
				break;
			else {
                                int j;
				for( j=0; j < d; j++ )
					m_szLine[i+j] = m_bufRowDelimiter[j]; // Build line
				i += j;
			}
		}
	}
	else if( Parser::m_pBuffer ) {
		for( i=0; Parser::m_uiBytesProcessed < Parser::m_uiTotalBytes;  ) {		
			for( d=0; d < m_RowDelimiterLen && Parser::m_uiBytesProcessed < Parser::m_uiTotalBytes; d++ ) {
				m_bufRowDelimiter[d] = Parser::m_pBuffer[Parser::m_uiBytesProcessed++];

				if( m_szRowDelimiter[d] == m_bufRowDelimiter[d] )
					bBailout = true;
				else {
					bBailout = false;
					d++;
					break;
				}
			}
			if( bBailout == true )
				break;
			else {
                            int j;
				for( j=0; j < d; j++ )
					m_szLine[i+j] = m_bufRowDelimiter[j]; // Build line
				i += j;
			}
		}
	}
	m_szLine[i] = '\0'; // NULL terminate line
	

	// return false if nothing was read and we're at the end of the file
	if( Parser::m_fp != NULL ) 	{
		if( !i && feof(Parser::m_fp) )
			return false;
	}
	else if( Parser::m_pBuffer ) 	{
		if( !i && Parser::m_uiBytesProcessed >= Parser::m_uiTotalBytes )
			return false;
	}

	return true;
}


bool RowParser::ParseNextRow() {
	m_vecRow.clear();
    int linelen;

    // Look for a text string at the beginning of the line that matches the start marker string
    // if we are using this to either skip some junk or load the headers from a specific line.
    do {
	    if( !GetNextLine() )
		    return false;

	    linelen = strlen( m_szLine );

        if ( linelen < m_strStartMarker.size() )
            continue;

        if ( strncmp ( m_szLine, m_strStartMarker.c_str(), m_strStartMarker.size() ) == 0 )
            m_bStartMarkerFound = true;

    } while ( m_bUseStartMarker && !m_bStartMarkerFound );

	//char	a;
	bool	bBailout = false;
	int		d;
	int		i=0;
	int		start_marker = 0, end_marker = 0;

    // Quoted strings indicate that a delimiter within quotes is ignored.
	bool	bOpenQuote = false;


    while( i < linelen ) {		
		for( d=0; d < m_ItemDelimiterLen && i < linelen; d++ ) {
			if( ( m_bufItemDelimiter[d] = m_szLine[i++]) == '\"' )
			{
				if( m_bStringsQuoted ) {
					if( bOpenQuote = (bOpenQuote ? false : true) )
						start_marker++;
					else
						m_szLine[i-1] = '\0';
				}
			}

			if( !bOpenQuote ) {
				if( m_szItemDelimiter[d] == m_bufItemDelimiter[d] ) {
					bBailout = true;
				}
				else {
					bBailout = false;
					break;
				}
			}
            else {
					bBailout = false;
					break;
			}
		}
		if( i == linelen || bBailout == true ) {
			m_szLine[i-d] = '\0';
			m_vecRow.push_back( &(m_szLine[start_marker]) );
			start_marker = i;
		}
	}

    // Now, if we are supposed to return pairs, and the vector has only 1 value in it,
    // push an empty value.  This is because there was a key=value pair and there was no value,
    // only a key= was found.  If there are no values in this row, push 2 empty values.
    if ( m_bParsePairs && m_vecRow.size() == 0 )
        m_vecRow.push_back( "" );
    if ( m_bParsePairs && m_vecRow.size() == 1 )
        m_vecRow.push_back( "" );

	if( m_bUseHeaders == true && m_bHeadersLoaded == false ) {
        // Check to see if we are to use the first (0) position or not...
        // If not, erase it.
        if ( !m_bIncStartMarker )
            m_vecRow.erase(m_vecRow.begin());
		for( int iHeader = 0; iHeader < m_vecRow.size(); iHeader++ )
		{
			m_mapHeaders[ m_vecRow[iHeader] ] = iHeader;
                        m_mapHeaderLookup[ iHeader ] = m_vecRow[iHeader];
		}

		m_bHeadersLoaded = true;

		return RowParser::ParseNextRow();
	}

	return true;
}

void RowParser::Reset() {
	m_bHeadersLoaded = false;

	Parser::Reset();
}

// Statistical functions
int RowParser::GetNumRowsProcessed() {
	return m_iRowCounter;
}

int RowParser::GetFirstRowItemCount() {
	return m_iFirstRowItemCount;
}

int RowParser::GetLastRowItemCount() {
	return m_iLastRowItemCount;
}

int RowParser::GetMinimumRowItemCount() {
	return m_iMinimumRowItemCount;
}

int RowParser::GetMaximumRowItemCount() {
	return m_iMaximumRowItemCount;
}

// Operator overloads
std::string RowParser::operator[]( int iIndex ) {
	if( iIndex >= m_vecRow.size() ) 	{
		throw RowParserException( "Index out of bounds" );
	}
		
	return m_vecRow[iIndex]; //.c_str();
}

std::string RowParser::operator[]( const char* szHeader ) {
	return m_vecRow[ m_mapHeaders[szHeader] ]; //.c_str();
}



