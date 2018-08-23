//===========================================================================
// Copyright EasyLogic 2006
//---------------------------------------------------------------------------
// DConfigFile -- Read and save a configuratie bestand
//---------------------------------------------------------------------------
// Coder            | Date          | Description
// N. Rice	    | 18-04-016	    | Creation
//===========================================================================

//===========================================================================
// HEADER
//===========================================================================

#ifndef DCONFIGFILE_HEADER
#define DCONFIGFILE_HEADER


#include <vcl.h>
#include <stdio.h>
//#pragma hdrstop



//===========================================================================
// Class Definition
//===========================================================================
class DConfigFile
    {
    public:
	// constructor/destructor
	DConfigFile(void);
	DConfigFile(char * strFileName);
	~DConfigFile(void);

	// file mananagement
	bool    openTextFile(char * strFileName);
	bool    createTextFile(char * strFileName);
	void    closeTextFile(void);

	// read and search options
	bool    readString(char * strVariable, char * strValue, char * strDefault);
	bool    readInteger(char * strVariable, int * piValue, int iDefault);
	bool    readBoolean(char * strVariable, bool * pbValue, bool bDefault);
     /*
	bool    writeString(char * strVariable, char * strValue);
	bool    writeInteger(char * strVariable, int iValue);
	bool    writeBoolean(char * strVariable, bool bValue);
       */
//    protected:
	bool    bFileOpen;

       

	HANDLE  hFileHandle;
	BYTE *  pBuffer;
	DWORD   dwSize;
        char	storeFileName[256];
	bool    readLine(char * strLine, char * strValue, DWORD * pdwStartPosition);
	bool    writeLine(char * strLine, DWORD dwStartPosition, DWORD dwEndPosition);

    private:

    };

//---------------------------------------------------------------------------
#endif
