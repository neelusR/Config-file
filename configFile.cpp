//===========================================================================
//---------------------------------------------------------------------------
// configFile 
//---------------------------------------------------------------------------
// Coder            | Date          | Description
// N. Rice	    | 18-04-06      | Creation
//===========================================================================

//===========================================================================
// HEADER
//===========================================================================
//---------------------------------------------------------------------------
// includes
//---------------------------------------------------------------------------
//#include "stdafx.h"

//#include <vcl.h>
//#include <commctrl.h>
//#include <aygshell.h>
//#include <sipapi.h>
#include <stdio.h>
//#include <ansistring.h>
//#pragma hdrstop

#include "configfile.h"

//---------------------------------------------------------------------------

//#pragma package(smart_init)


//===========================================================================
// Class Members
//===========================================================================

//---------------------------------------------------------------------------
// DConfigFile: Constructor
//---------------------------------------------------------------------------
DConfigFile :: DConfigFile(void)
{
bFileOpen = false;

} // Constructor

//---------------------------------------------------------------------------
// DConfigFile: Constructor - met openen van bestand
//---------------------------------------------------------------------------
DConfigFile :: DConfigFile(char * strFileName)
{
openTextFile(strFileName);

} // Constructor

//---------------------------------------------------------------------------
// DConfigFile: Destructor
//---------------------------------------------------------------------------
DConfigFile :: ~DConfigFile(void)
{
if (bFileOpen)
	{
	CloseHandle(hFileHandle);
	bFileOpen = false;
	delete[] pBuffer;
	}

} // Destructor

//---------------------------------------------------------------------------
// DConfigFile: openTextFile -- Open a text file
// opens file, gets file size, creates buffer, reads complete file into buffer
// if error returns a false
//---------------------------------------------------------------------------
bool DConfigFile :: openTextFile(char * strFileName)
{
unsigned long   dwBytesRead;

strcpy( storeFileName,strFileName);	//save filename

if (bFileOpen)
	{
	CloseHandle(hFileHandle);
	delete[] pBuffer;
	bFileOpen = false;
	}


 //normal method
hFileHandle = CreateFile(strFileName,
			     GENERIC_READ,
			     FILE_SHARE_READ | FILE_SHARE_WRITE,
			     NULL,
			     OPEN_EXISTING,
			     0,
			     NULL);

 /*
//special for embedded
hFileHandle = CreateFile(TEXT("\\IPAQ File Store\\Tagreader\\config.cfg"),
			     GENERIC_READ,
			     FILE_SHARE_READ | FILE_SHARE_WRITE,
			     NULL,
			     OPEN_EXISTING,
			     0,
			     NULL);

 */
if (hFileHandle == INVALID_HANDLE_VALUE)
	{
	// file could not be opened
	return false;
	}

dwSize = GetFileSize(hFileHandle, NULL);
pBuffer = new BYTE[dwSize];
ReadFile(hFileHandle, pBuffer,dwSize, &dwBytesRead, NULL);
	     
if (dwBytesRead != dwSize)
	{
	// error in read
	delete[] pBuffer;
	CloseHandle(hFileHandle);
	return false;
	}

bFileOpen = true;
return true;
} // openTextFile


//---------------------------------------------------------------------------
// DConfigFile: createTextFile -- Open a text file
// opens file if error opening creates file
// 
//---------------------------------------------------------------------------
bool DConfigFile :: createTextFile(char * strFileName)
{
unsigned long   dwBytesRead;


if ( !openTextFile(strFileName))
	{
	strcpy( storeFileName,strFileName);	//save filename

	if (bFileOpen)
		{
		CloseHandle(hFileHandle);
		delete[] pBuffer;
		bFileOpen = false;
		}

 //normal
	hFileHandle = CreateFile(strFileName,
			     GENERIC_READ,
			     FILE_SHARE_READ | FILE_SHARE_WRITE,
			     NULL,
			     CREATE_ALWAYS,
			     0,
			     NULL);

  /*	//special for embedded
hFileHandle = CreateFile(TEXT("\\IPAQ File Store\\Tagreader\\config.cfg"),
			     GENERIC_READ,
			     FILE_SHARE_READ | FILE_SHARE_WRITE,
			     NULL,
			     CREATE_ALWAYS,
			     0,
			     NULL);

     */
	if (hFileHandle == INVALID_HANDLE_VALUE)
		{
		// file could not be opened
		return false;
		}

	dwSize = GetFileSize(hFileHandle, NULL);
	pBuffer = new BYTE[dwSize];
		 
    
	bFileOpen = true;
	}

return true;
}

//---------------------------------------------------------------------------
// DConfigFile: closeTextFile -- Close a file
//---------------------------------------------------------------------------
void DConfigFile :: closeTextFile(void)
{
if (bFileOpen)
	{
    CloseHandle(hFileHandle);
    bFileOpen = false;
    delete[] pBuffer;
    }

} // closeTextFile

//---------------------------------------------------------------------------
// DConfigFile: readString -- search for a certan string variable
// gets variable through realine()
//---------------------------------------------------------------------------
bool DConfigFile:: readString  (char * strVariable, char * strValue,  char * strDefault )
{
char            strLine[256];
char            strValueFound[256];
unsigned long   dwPosition = 0;
int			iCounter;
char	CTemp1[256];
char	CTemp2[256];
char	CCompare[256];
int             iPos;

//strcpy(CCompare, strVariable);	//copy pointer to strVariable
// search for the string
while (readLine(strLine, strValueFound, &dwPosition))	//get next string variable, returns string in strLine, and new pos in dwPosition
	{
	if (strcmp(strLine, strVariable) == 0)	//check if variable is found
		{
		strcpy(strValue,strValueFound);
		return true;
		}
	}



// not found
strcpy(strValue, strDefault);
return false;
} // readString

//---------------------------------------------------------------------------
// DConfigFile: readInteger -- search for a certan integer variable
// returns true if success, 
//---------------------------------------------------------------------------
bool DConfigFile :: readInteger  ( char * strVariable,  int *  piValue,  int    iDefault  )
{
char            strLine[256];
char            strValueFound[256];
unsigned long           dwPosition = 0;
int             iPos;

while (readLine(strLine, strValueFound, &dwPosition))	//get next string variable, returns string in strLine, and new pos in dwPosition
	{
	if (strcmp(strLine, strVariable) == 0)	//check if variable is found
		{
		*piValue = atoi(strValueFound);
		return true;
		}
	}


// not found
*piValue = iDefault;
return false;
} // readInteger

//---------------------------------------------------------------------------
// DConfigFile: readBoolean -- search for a certan boolean variable
//---------------------------------------------------------------------------
bool DConfigFile :: readBoolean   ( char * strVariable,  bool * pbValue,   bool   bDefault  )
{
char            strLine[256];
char            strValueFound[256];
unsigned long           dwPosition = 0;
int             iPos;


while (readLine(strLine, strValueFound, &dwPosition))	//get next string variable, returns string in strLine, and new pos in dwPosition
	{
	if (strcmp(strLine, strVariable) == 0)	//check if variable is found
		{
		if (strcmp(strValueFound,"TRUE")== 0 ||( strcmp (strValueFound,"YES")==0) || (strValueFound[0] == 'Y') || (strValueFound[0] == 'T'))
			{
			*pbValue = true;
			return true;
			}
		if ( (strcmp(strValueFound,"FALSE") == 0) || (strcmp(strValueFound,"NO") == 0 ) || (strValueFound[0] == 'N') || (strValueFound[0] == 'F'))
			{
            *pbValue = false;
			return true;
			}

	
		}
	}


// not found
*pbValue = bDefault;
return false;
} // readBoolean

//---------------------------------------------------------------------------
// DConfigFile: readLine -- read a line from the file
// gets first variable from given StartPosition and returns in strLine
// returns position in buffer through StartPosition
// if error returns false, if succes true
//  format: first char #
//---------------------------------------------------------------------------
bool DConfigFile :: readLine  ( char * strLine,  char * strValue, unsigned long * pdwStartPosition  )
{
unsigned long   dwFirstChar;
int     iLineCounter;
int     iState = 0;
bool    bInString = false;

// test if file is open
if (!bFileOpen)
	{
	// file not open
    return false;
    }

// search for a string
for (dwFirstChar = *pdwStartPosition, iLineCounter = 0;  dwFirstChar < dwSize;  dwFirstChar++)
	{
	switch(iState)
		{
	    case 0:
		if (pBuffer[dwFirstChar] == '#')      // search for startChar
			{
			iState++;
	    }
		else if ((pBuffer[dwFirstChar] != 13) && (pBuffer[dwFirstChar] != 10))
		    {                  // error, no CR and LF, search next line
		    iState = 10;
		    }
		break;
	    
		case 1:
		// start reading line
		if (iLineCounter == 255)
		    {		    // error, data string to long
		    iState = 10;
		    iLineCounter = 0;
		    }
		else if ( pBuffer[dwFirstChar] == '=')		//separator detected
			{
			strLine[iLineCounter] = 0;		//terminate string with zero
			iState++;
			iLineCounter =0;
			}
		else if ((!bInString) && (pBuffer[dwFirstChar] == ' ' || pBuffer[dwFirstChar] == 9)  )	//ignore spaces and tabs
			{

			}
		else		      //no error
		    {               //place char in return buf and increase counter
		    strLine[iLineCounter++] = pBuffer[dwFirstChar];
		    }
		break;

		case 2:
		if ((bInString) &&  (pBuffer[dwFirstChar] == '"'))
		    {					//end of string data detected
		    bInString = false;
		    }
		else if ((!bInString) && (pBuffer[dwFirstChar] == '"'))
		    {					//entry of string data detected
		    bInString = true;
		    }
		else if ((!bInString) && (pBuffer[dwFirstChar] == ';'))
		    {                               //end of variable definition
		    strValue[iLineCounter] = 0;		//terminate string with zero
		    *pdwStartPosition = dwFirstChar + 1;	//place new location pointer
		    return true;
		    }
		else if (iLineCounter == 255)
		    {		    // error, data string to long
		    iState = 10;
		    iLineCounter = 0;
		    }
		else if ((!bInString) && (pBuffer[dwFirstChar] == ' ' || pBuffer[dwFirstChar] == 9))	//when outside of string ignore spaces
			{

			}
		else		      //no error
		    {               //place char in return buf and increase counter
		    strValue[iLineCounter++] = pBuffer[dwFirstChar];
		    }
		break;

		
		case 10:
		if ((pBuffer[dwFirstChar] == 10) || (pBuffer[dwFirstChar] == 13))    // read until next line
		    {                       //found next line, goto begin
		    iState = 0;
		    }
		break;
	    }
	}

// no more vars found
return false;
} // readLine



//---------------------------------------------------------------------------
// DConfigFile: writeString -- search for a certan string variable
// returns true if success, 
//---------------------------------------------------------------------------
bool DConfigFile :: writeString  ( char * strVariable,  char * strValue )
{
char            strLine[256];
char            strValueFound[256];
char		strNewValue[256];
unsigned long   dwBeginPosition, dwEndPosition = 0;
int             iPos, iOldSize, iNewSize;
char	    	tmpValue[256];

while (readLine(strLine, strValueFound, &dwEndPosition))
	{
	if (strcmp(strLine, strVariable) == 0)	//check if variable is found
		{
		sprintf(strNewValue," \"%s\";",strValue);
		if (writeLine ( strNewValue, dwEndPosition))
			return true;
		}
	}


/*
char            strLine[256];
unsigned long           dwBeginPosition, dwEndPosition = 0;
AnsiString      CTemp1;
AnsiString      CTemp2;
AnsiString      CCompare;
int             iPos, iSize;
char	    tmpValue[256];

CCompare = strVariable;
// search for the string
while (readLine(strLine, &dwEndPosition))
	{
	// check the var
	CTemp1 = strLine;
	iPos = CTemp1.AnsiPos("=");       //Locates the position of a substring within an AnsiString.
	if (iPos > 1)
	    {
	    CTemp1 = CTemp1.SetLength(iPos - 1);      //place variable name in CTemp1
	    CTemp1 = CTemp1.Trim();
	    if (CCompare == CTemp1)
			{
			iSize = StrLen( strLine );
			dwBeginPosition = (dwEndPosition - iSize) -3;  //including 2 char for " "
			dwEndPosition--;
			strLine[iPos+1] = 0x22;   ///  " char
			sprintf(&strLine[iPos+2],"%s", strValue);
			iSize = StrLen( strLine );
			strLine [iSize] = 0x22;
			strLine [iSize+1] = 0;
					
			if (writeLine (strLine, dwBeginPosition, dwEndPosition))
				return true;
			}
	    }
	}
  */
return false;
} // writeString


//---------------------------------------------------------------------------
// DConfigFile: readInteger -- search for a certan integer variable
// returns true if success, 
//---------------------------------------------------------------------------
bool DConfigFile :: writeInteger  ( char * strVariable,  int iValue )
{
char            strLine[256];
char            strValueFound[256];
char		strNewValue[256];
unsigned long   dwBeginPosition, dwEndPosition = 0;
int             iPos, iOldSize, iNewSize;
char	    	tmpValue[256];

while (readLine(strLine, strValueFound, &dwEndPosition))
	{
	if (strcmp(strLine, strVariable) == 0)	//check if variable is found
		{
		sprintf(strNewValue," %d;",iValue);
		if (writeLine ( strNewValue, dwEndPosition))
			return true;
		}
	}

return false;
} // writeInteger


//---------------------------------------------------------------------------
// DConfigFile: readBoolean -- search for a certan Boolean variable
// returns true if success, 
//---------------------------------------------------------------------------
bool DConfigFile :: writeBoolean  ( char * strVariable,  bool bValue )
{
char            strLine[256];
char            strValueFound[256];
char		strNewValue[256];
unsigned long   dwBeginPosition, dwEndPosition = 0;
int             iPos, iOldSize, iNewSize;
char	    	tmpValue[256];

while (readLine(strLine, strValueFound, &dwEndPosition))
	{
	if (strcmp(strLine, strVariable) == 0)	//check if variable is found
		{
		if (bValue)
			sprintf(strNewValue," Y;");
		else
			sprintf(strNewValue," N;");		
		if (writeLine ( strNewValue, dwEndPosition))
			return true;
		}
	}

/*
char            strLine[256];
unsigned long           dwBeginPosition, dwEndPosition = 0;
AnsiString      CTemp1;
AnsiString      CTemp2;
AnsiString      CCompare;
int             iPos, iSize;
char	    tmpValue[256];

CCompare = strVariable;
// search for the string
while (readLine(strLine, &dwEndPosition))
	{
	// check the var
	CTemp1 = strLine;
	iPos = CTemp1.AnsiPos("=");       //Locates the position of a substring within an AnsiString.
	if (iPos > 1)
	    {
	    CTemp1 = CTemp1.SetLength(iPos - 1);      //place variable name in CTemp1
	    CTemp1 = CTemp1.Trim();
	    if (CCompare == CTemp1)
			{
			iSize = StrLen( strLine );
			dwBeginPosition = (dwEndPosition - iSize) -1;
			dwEndPosition--;
			if (bValue)
				sprintf(&strLine[iPos+1],"TRUE");
			else
				sprintf(&strLine[iPos+1],"FALSE");

			if (writeLine (strLine, dwBeginPosition, dwEndPosition))
				return true;
			}
	    }
	}
*/

return false;
}//writeBoolean 



//---------------------------------------------------------------------------
// DConfigFile: writeLine -- writes a line to buffer
//		writeLine ( strLine, strNewValue, iOldSize, iNewSize);
//---------------------------------------------------------------------------
bool DConfigFile :: writeLine  ( char * strNewValue,  unsigned long dwEndPosition )
{
unsigned long   dwFirstChar, dwTmpSize,dwBytesWritten, dwStartPosition;
int     iLineCounter;
long	iCounter;
BYTE *  pTmpBuffer;	
bool    bFail;
int	iNewSize;
int	iRealOldSize;

bFail = false;

// test if file is open
if (!bFileOpen)
	{
	// file not open
	return false;
	}
iRealOldSize = 0;
				   //get real size of old value
for (dwFirstChar = dwEndPosition;  dwFirstChar > 0;  dwFirstChar--)
	{                         //place first part of buffer
	if (pBuffer[dwFirstChar] == '=')
		break;
	}
dwFirstChar++;
iRealOldSize = dwEndPosition - dwFirstChar;

 
iNewSize=StrLen(strNewValue);

//create new buffer based on new string size
dwTmpSize = dwSize + (iNewSize - iRealOldSize );
pTmpBuffer = new BYTE[dwTmpSize];


//place old data
for (iLineCounter = 0;  iLineCounter < dwFirstChar ;  iLineCounter++)
	{                         //place first part of buffer
	pTmpBuffer[iLineCounter] = pBuffer[iLineCounter];
	}

//place new data	
for (iCounter = 0;  iCounter < iNewSize ;  iCounter++)
	{                         
	pTmpBuffer[iLineCounter+iCounter] = strNewValue[iCounter];
	}
iLineCounter += iCounter;

//place 2nd part
for ( ;  dwEndPosition < dwSize ;  dwEndPosition++)
	{                         
	pTmpBuffer[iLineCounter++] = pBuffer[dwEndPosition];
	}
	

	if (bFileOpen)				//close original file and buffer
		{
		CloseHandle(hFileHandle);
		delete[] pBuffer;
		bFileOpen = false;
		}

		
//normal
	hFileHandle = CreateFile(storeFileName,
			     GENERIC_WRITE,
			     FILE_SHARE_WRITE,
			     NULL,
			     OPEN_ALWAYS,
			     0,
			     NULL);
 
  
  /*	//special for embedded
hFileHandle = CreateFile(TEXT("\\IPAQ File Store\\Tagreader\\config.cfg"),
			     GENERIC_WRITE,
			     FILE_SHARE_WRITE,
			     NULL,
			     OPEN_ALWAYS,
			     0,
			     NULL);

     */
			     
	if (hFileHandle == INVALID_HANDLE_VALUE)
		bFail = true;
	else				//write new buffer to file
		{
		bFileOpen = true;

		SetFilePointer(hFileHandle, 0, NULL, FILE_BEGIN);

		if (WriteFile(hFileHandle, pTmpBuffer,  dwTmpSize, &dwBytesWritten, NULL) == 0)
			bFail = true;

		CloseHandle(hFileHandle);		
		bFileOpen = false;    
		}
   //	}
	

if ( openTextFile(storeFileName) == false )
	bFail = true;    
	
	
delete[] pTmpBuffer;

if (	bFail == true)
   return false;
else
   return true;
} // writeLine



