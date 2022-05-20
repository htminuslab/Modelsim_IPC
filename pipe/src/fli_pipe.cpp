//-------------------------------------------------------------------------------------------------
// Simple FLI demo using named pipe 
//
// https://github.com/htminuslab            
//                                       
//-------------------------------------------------------------------------------------------------
//  Revision History:                                                        
//                                                                           
//  Date:        Revision    Author         
//  01 Sep 2019  0.1         Hans Tiggeler 
//  19 May 2022  0.2         Hans Tiggeler  Added pipe_name and checked with  Modelsim 2022.2
//-------------------------------------------------------------------------------------------------
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>

#include <stdio.h>
#include <string.h>

#include <mti.h>

#define MAX_BUF 			1024
#define TIME_OUT_SECONDS	300

HANDLE pipe=INVALID_HANDLE_VALUE;

//-------------------------------------------------------------------------------------------------
// Convert VHDL string into a NULL terminated C string 
//-------------------------------------------------------------------------------------------------
static char *get_string(mtiVariableIdT id)
{
	static char buf[1000];
	mtiTypeIdT type;
	int len;
	mti_GetArrayVarValue(id, buf);
	type = mti_GetVarType(id);
	len = mti_TickLength(type);

	buf[len] = 0;
	return buf;
}

//-------------------------------------------------------------------------------------------------
// Open named pipe
// Return false if failed to open pipe.
//-------------------------------------------------------------------------------------------------
bool open_pipe(mtiVariableIdT pipe_name, int timeout)
{	
	do {	
		pipe = CreateFile(get_string(pipe_name), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE){
			mti_PrintFormatted("\nPipe %s not yet open (%ld), waiting...", get_string(pipe_name), GetLastError());
			Sleep(1000);											// wait a second, then try again			
			timeout--;
		} else {
			mti_PrintFormatted("\nPipe %s is now open\n", get_string(pipe_name));
			return true;											// Pipe open
		}
	} while(timeout);	

	return false;													// Time out failure
}

//-------------------------------------------------------------------------------------------------
// Read pipe
// Valid data if numread!=0
//-------------------------------------------------------------------------------------------------
void read_pipe(DWORD *numread, mtiVariableIdT vector)
{
	char data[MAX_BUF];
	int len;
	char *val;
	
	len = mti_TickLength(mti_GetVarType(vector)); 					// get length 
	
	ReadFile(pipe, data, MAX_BUF, numread, NULL);
	if (numread > 0) {
		val = (char *)mti_GetArrayVarValue(vector, NULL);			
		for (int i = 0; i < len; i++) val[i] = data[i];
	}
}

void close_pipe(void)
{
	CloseHandle(pipe);	
	mti_PrintMessage("Pipe closed\n");
}
