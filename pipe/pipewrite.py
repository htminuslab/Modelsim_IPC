# -------------------------------------------------------------------------------------------------
#  Simple FLI demo using named pipe 
#
#  pip install pywin32
# 
#  https://github.com/htminuslab            
#                                        
# -------------------------------------------------------------------------------------------------
#   Revision History:                                                        
#                                                                            
#   Date:        Revision    Author         
#   01 Sep 2019  0.1         Hans Tiggeler 
#   19 May 2022  0.2         Hans Tiggeler  Added pipe_name and checked with  Modelsim 2022.2
# -------------------------------------------------------------------------------------------------
#import os
#import csv
#import sys
import win32pipe, win32file
import time
INVALID_HANDLE_VALUE = 0

pipe = 0

def open_pipe(pipe_name):
    global pipe
    try:
        pipe = win32pipe.CreateNamedPipe(
            pipe_name,
            win32pipe.PIPE_ACCESS_DUPLEX,
            win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_READMODE_MESSAGE | win32pipe.PIPE_WAIT,
            1, 65536, 65536, 0, None)
        print("Waiting for pipe",pipe_name,"to open...")
        win32pipe.ConnectNamedPipe(pipe, None) # blocking
        return pipe
                  
    except:
        return INVALID_HANDLE_VALUE
         
def write_pipe(message):   
    global pipe
    try:
        win32file.WriteFile(pipe, bytes(message+'\0', 'UTF-8'))
    except:
        print("Failed to write to pipe")  


def main():                                   
    
    #------------------------------------------------------------------------------
    # Open named pipe, wait until opened
    #------------------------------------------------------------------------------    
    pipe=open_pipe('\\\\.\\pipe\\LogPipe')
    if pipe==INVALID_HANDLE_VALUE:
        print("Failed to open Pipe")
        quit()
    
    #------------------------------------------------------------------------------
    # Send 128bits vector every second
    #------------------------------------------------------------------------------            
    for v in range(182841384165841685416854134135,914206920829208427084270670675,182841384165841685416854134135):
        print("Sending data",f'{v:0128b}')
        write_pipe(f'{v:0128b}')     
        time.sleep(1)       

    #------------------------------------------------------------------------------
    # Tell Modelsim we are done
    #------------------------------------------------------------------------------  
    write_pipe("Quit") 

if __name__=='__main__':
    main() 