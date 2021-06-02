import pyBlock
import os

from hwgrader.module_utils import *
from hwgrader.test_utils import *
from hwgrader.utils import ParTextTestRunner, ParTestCase, RebootableTestSuite
import fcntl
import unittest
import sys
import errno

def test1():
    my_pid = os.getpid()
    fork = tfork2()
    if fork.isChild:
        print("child pid is:" + str(os.getpid()))
        fork.sync()
        
        print("test21")
        try: 
           open("restricted.txt")
        except Exception , ex:
            print(ex) 
            print("test21 fail !!!!!!!!!!!!!!!!!!!ed")
                
        fork.exit()
  

    

        
    print("parent pid is:" + str(os.getpid()))    
    ret = pyBlock.block_add_process(os.getpid())
    pyBlock.block_add_file("restricted.txt")
    if not ret ==1:
        print("failed!!!!!!!!!!!!!!!")    
    
    print("test21")
    try: 
       ret = pyBlock.block_add_process(fork.cpid)
       if not ret ==2:
            print("test21 fail!!!!!!!!!!!!!!!")  
    except Exception , ex:
        print(ex) 
        print("test21 fail !!!!!!!!!!!!!!!!!!!ed")
        
    print("test23")
    try: 
       ret = pyBlock.block_add_process(13234322)
       if not ret ==2:
            print("test21 fail!!!!!!!!!!!!!!!") 
       print("test23 fail !!!!!!!!!!!!!!!!!!!ed")
    except Exception , ex:
        print(ex) 
    
    print("test24")
    try: 
       tmp = pyBlock.block_query(None)
       if not tmp==0:
           print("test24 fail !!!!!!!!!!!!!!!!!!!ed")
    except Exception , ex:
        print(ex) 
        
    print("test25")
    try: 
       pyBlock.block_add_file(None)
       print("test25 fail !!!!!!!!!!!!!!!!!!!ed")
    except Exception , ex:
        print(ex)
        
        
    print("test26")
    try: 
        pyBlock.block_add_file("restricted2.txt")
        pyBlock.block_add_file("restricted3.txt")
        pyBlock.block_add_file("restricted4.txt")
        tmp2 = pyBlock.block_query("restricted2.txt")
        tmp3 = pyBlock.block_query("restricted3.txt")
        tmp4 = pyBlock.block_query("restricted2.txt")
        if not tmp2 == 1 and not tmp3 == 1 and not tmp4 == 1: 
            print("test26 fail !!!!!!!!!!!!!!!!!!!")
        open("restricted3.txt")
    except Exception , ex:
        print(ex)
        print("test26 fail !!!!!!!!!!!!!!!!!!!")
        
    print("test26b")
    try: 
        open("restricted4")
        print("test26b fail !!!!!!!!!!!!!!!!!!!")
    except Exception , ex:
        print(ex)
        
    print("test27")
    try: 
        pyBlock.block_clear()
        tmp2 = pyBlock.block_query("restricted2.txt")
        tmp3 = pyBlock.block_query("restricted3.txt")
        tmp4 = pyBlock.block_query("restricted4.txt")
        if not tmp2 == 0 and not tmp3 == 0 and not tmp4 == 0: 
            print("test26 fail !!!!!!!!!!!!!!!!!!!")
        pyBlock.block_add_file("restricted2.txt")
        pyBlock.block_add_file("restricted3.txt")
        pyBlock.block_add_file("restricted4.txt")
        tmp2 = pyBlock.block_query("restricted2.txt")
        tmp3 = pyBlock.block_query("restricted3.txt")
        tmp4 = pyBlock.block_query("restricted4.txt")
        if not tmp2 == 1 and not tmp3 == 1 and not tmp4 == 1: 
            print("test26 fail !!!!!!!!!!!!!!!!!!!")
    except Exception , ex:
        print(ex)
        print("test27 fail !!!!!!!!!!!!!!!!!!!")
     
    print("test28")
    try: 
        pyBlock.block_clear()
        pyBlock.block_clear()
    except Exception , ex:
        print(ex)
        print("test28 fail !!!!!!!!!!!!!!!!!!!")
    
    print("test29")
    try: 
        tmp2 = pyBlock.block_query("restricted2.txt")
        if not tmp2 == 0:
            print("test29 fail !!!!!!!!!!!!!!!!!!!")
    except Exception , ex:
        print(ex)
        print("test29 fail !!!!!!!!!!!!!!!!!!!")
             
        
        
if __name__ == "__main__":
    test1()