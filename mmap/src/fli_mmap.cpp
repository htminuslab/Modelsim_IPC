//-------------------------------------------------------------------------------------------------
// Simple FLI demo using shared memory
//
// https://github.com/htminuslab            
//                                       
//-------------------------------------------------------------------------------------------------
//                                                    
//-------------------------------------------------------------------------------------------------
// 
//  Revision History:                                                        
//                                                                           
//  Date:        Revision    Author         
//  19 August    2019  0.1   Hans Tiggeler 
//-------------------------------------------------------------------------------------------------
#include <windows.h>
#include <cstring>
#include <cstdbool>
#include <stdio.h>
#include <mti.h>

#define STRING_LENGTH	128

typedef struct
{
    void* hFileMap;
    void* pData;
    char MapName[256];
    size_t Size;
} SharedMemory;

SharedMemory shmx = {0};
SharedMemory *shm=&shmx;

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


bool CreateMemoryMap(mtiVariableIdT mmap_name) 
{
    shm->Size = 512;

	sprintf(shm->MapName, get_string(mmap_name));
	mti_PrintFormatted("FLI Create Memory Map %s\n",shm->MapName);

    if ((shm->hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, shm->Size, shm->MapName)) == NULL) {
        mti_PrintMessage("FLI CreateFileMapping Failed\n");
		return false;
    }

    if ((shm->pData = MapViewOfFile(shm->hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, shm->Size)) == NULL) {
        CloseHandle(shm->hFileMap);
		mti_PrintMessage("FLI MapViewOfFile Failed\n");
        return false;
    }	
    return true;
}

bool FreeMemoryMap(void)
{
    if (shm && shm->hFileMap) {
        if (shm->pData) UnmapViewOfFile(shm->pData);
        if (shm->hFileMap) CloseHandle(shm->hFileMap);       
		mti_PrintMessage("FLI Memory Map released\n");
        return true;
    }
    return false;
}

void ReadMemoryMap(bool *valid, mtiVariableIdT vector)
{
	static int rdptr=0, wrptr=0;	
	char* ptr = (char*)shm->pData;
	int len;
	char *val;
	
	len = mti_TickLength(mti_GetVarType(vector)); 	// get length 
	
	*valid=false;
	if (!(ptr && (*ptr == 0))) {					// Wait for Data		
		wrptr=strlen(ptr);		
		if (rdptr<wrptr) {
			val = (char *)mti_GetArrayVarValue(vector, NULL);			
			for (int i = 0; i < len; i++){
				val[i] = ptr[rdptr+i];
			}				
			rdptr+=STRING_LENGTH;	
			*valid=true;
		} 			
	}		
}
