//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "configfile.h"
#include "dir.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
DConfigFile *   pConfig;
int iRollNumber;
char str[256];
char strValue[256];
char configPath[256];
bool bValue;
myLabel->Caption = "hello";

getcwd( configPath, 256);             				//set path for config file
strcpy(&configPath[strlen(configPath)],"\\config.cfg");


pConfig = new DConfigFile;
if (!pConfig->openTextFile(configPath) )
	{
	myLabel->Caption = "error opening file";
	}
else
	{
	
	if (pConfig->readInteger("ROLLNUMBER", &iRollNumber,1))
		{
		sprintf(str,"succes: %d", iRollNumber);
		myLabel->Caption = str;		
		}
	else
		{
		sprintf(str,"error: %d", iRollNumber);
		myLabel->Caption = str;		
		}

	if (pConfig->readString("BARCODENUMBER", strValue, 0))
		{
		sprintf(str,"succes: %s",strValue);
		myLabel2->Caption = strValue;		
		}
	else
		{
		sprintf(str,"error: %d", iRollNumber);
		myLabel2->Caption = strValue;		
		}


	if (pConfig->readBoolean("TEST", &bValue,false))
		{
		if (bValue )
			sprintf(str,"succes: value true");
		else
			sprintf(str,"succes: value false");		
		myLabel3->Caption = str;		
		}
	else
		{
		sprintf(str,"error: %d", iRollNumber);
		myLabel3->Caption = str;		
		}

		
	
	pConfig->writeInteger("ROLLNUMBER", 7);
	pConfig->writeBoolean("TEST", false);
	pConfig->writeString("BARCODENUMBER", "new string");
	}

delete pConfig;

}
//---------------------------------------------------------------------------

