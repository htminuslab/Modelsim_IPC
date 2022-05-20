#---------------------------------------------------------------------------------------------------
# Simple FLI IPC demo using Shared Memory
#
#  https://github.com/htminuslab   
#---------------------------------------------------------------------------------------------------
#
#---------------------------------------------------------------------------------------------------
# Update: Created 30/11/2017
# Update: Updated for Python 3.8 and Modelsim 2022.1 14/03/2022
#---------------------------------------------------------------------------------------------------

import mmap
import time

def main(): 

    try:
        # Open Shared memory
        shm = mmap.mmap(0, 512, "Local\\Test") 
        
		# Generate some testvectors and send to Modelsim
        if shm:       
            for v in range(182841384165841685416854134135,914206920829208427084270670675,182841384165841685416854134135):
                shm.write(bytes(f'{v:0128b}', 'UTF-8'))
                print("Sending value",f'{v:0128b}')
                time.sleep(1)
                shm.flush()
    except:
        print("Cannot open shared memory")  

if __name__=='__main__':
    main() 