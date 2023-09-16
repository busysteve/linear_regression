#include "parser.h"

Parser::Parser() {
	m_fp = NULL;
	m_pBuffer = NULL;
	m_pFunc = NULL;
	m_bContinue = true; 
	
	m_uiTotalBytes = 0;
    m_uiBytesProcessed = 0;

}

Parser::Parser( FILE* fpFile) {
	ParseFile(fpFile);
}

Parser::Parser(const char* szFilename) {
	ParseFile(szFilename);
}

Parser::Parser( char * pBuffer, unsigned int uiSize ) {
	ParseBuffer( pBuffer, uiSize );
}

Parser::~Parser() {
	if( m_fp )
		fclose( m_fp );
}

void Parser::ParseFile( FILE* fpFile) {
	fpos_t	filelength;

	m_fp = fpFile;

	// Get file size the multiplatform way

	//fseek(m_fp, 0, SEEK_END);		// Set filepointer to end of file

	//int flen = fgetpos(m_fp, &filelength);	// get file position ( same as size in bytes )

	//fseek(m_fp, 0, SEEK_SET);		// Set filepointer back to begining

	//SetTotalBytes(flen);	// Set total bytes for progress use
}

void Parser::ParseFile(const char* szFilename) {
	fpos_t	filelength;

	m_fp = fopen(szFilename, "rb");

	// Get file size the multiplatform way

	fseek(m_fp, 0, SEEK_END);		// Set filepointer to end of file

	int flen = fgetpos(m_fp, &filelength);	// get file position ( same as size in bytes )

	fseek(m_fp, 0, SEEK_SET);		// Set filepointer back to begining

	SetTotalBytes(flen);	// Set total bytes for progress use
}

void Parser::ParseBuffer( char * pBuffer, unsigned int uiSize ) {
	m_pBuffer = pBuffer;
	SetTotalBytes( uiSize );
}

void Parser::Reset() {
	if( m_fp != NULL )
		fseek( m_fp, 0, SEEK_SET );		// Set filepointer back to begining
}


// Callback utility functions
void Parser::Pause() {
	m_bContinue = false;
}

void Parser::Continue() {
	m_bContinue = true;
}

void Parser::SetTotalBytes( unsigned int uiTotal ) {
	m_uiTotalBytes = uiTotal;
}

unsigned int Parser::GetProcessedByteCount() {
	return m_uiBytesProcessed;
}

unsigned int Parser::GetTotalByteCount() {
	return m_uiTotalBytes;
}

int Parser::GetPercentComplete() {
	if( m_uiBytesProcessed && m_uiTotalBytes )
		return ( m_uiBytesProcessed * 100 ) / m_uiTotalBytes;
	return 0;
}

void Parser::SetCallBack( void pFunc() ) {
	m_pFunc = pFunc;
}


