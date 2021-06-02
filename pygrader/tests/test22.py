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
        
        print("test12")
        tmp1 = pyBlock.block_query("restricted.txt")
        try:
            pyBlock.block_clear()
            print("test12 fail !!!!!!!!!!!!!!!!!!!")
        except Exception , ex:
            print(ex)
            tmp2 = pyBlock.block_query("restricted.txt")
            if not tmp1 == 1 and not tmp2 == 1: 
                print("test12 fail !!!!!!!!!!!!!!!!!!!")
        
        print("test14")
        try: 
            pyBlock.block_add_file("restricted.txt")
            print("test14 fail !!!!!!!!!!!!!!!!!!!")
        except Exception , ex:
            print(ex)

        print("test22")
        try: 
            pyBlock.block_add_process(fork.ppid)
            print("test22 fail !!!!!!!!!!!!!!!!!!!")
        except Exception , ex:
            print(ex)
        
        fork.exit()
  

    

        
    print("parent pid is:" + str(os.getpid()))    
    ret = pyBlock.block_add_process(os.getpid())
    if not ret ==1:
        print("failed!!!!!!!!!!!!!!!")
    pyBlock.block_add_file("restricted.txt")
    
    print("test11")
    tmp1 = pyBlock.block_query("restricted.txt")
    pyBlock.block_clear()
    tmp2 = pyBlock.block_query("restricted.txt")
    if not tmp1 == 1 and not tmp2 == 0: 
        print("test11 fail !!!!!!!!!!!!!!!!!!!")
    
    print("test15")
    try: 
        pyBlock.block_add_file("restricted.txt")
        tmp = pyBlock.block_query("restricted.txt")
        if not tmp == 1: 
            print("test15 fail !!!!!!!!!!!!!!!!!!!")
    except Exception , ex:
        print(ex)   
        print("test15 fail !!!!!!!!!!!!!!!!!!!")            
    
    print("test13")
    try: 
        res = pyBlock.block_add_file(None)
        print(str(res))
        print("test13 fail !!!!!!!!!!!!!!!!!!!")
    except Exception , ex:
        print(ex)
    
    print("test16")
    try: 
        pyBlock.block_add_file("restricted2.txt")
        pyBlock.block_add_file("restricted3.txt")
        pyBlock.block_add_file("restricted4.txt")
        tmp2 = pyBlock.block_query("restricted2.txt")
        tmp3 = pyBlock.block_query("restricted3.txt")
        tmp4 = pyBlock.block_query("restricted4.txt")
        if not tmp2 == 1 and not tmp3 == 1 and not tmp4 == 1: 
            print("test16 fail !!!!!!!!!!!!!!!!!!!")
            
    except Exception , ex:
        print(ex) 
        print("test16 fail !!!!!!!!!!!!!!!!!!!")    
            
    print("test17")
    try: 
        pyBlock.block_add_file("restricted.txt")
        tmp = pyBlock.block_query("restricted.txt")
        if not tmp == 1: 
            print("test15 fail !!!!!!!!!!!!!!!!!!!")
    except Exception , ex:
        print(ex)   
        print("test17 fail !!!!!!!!!!!!!!!!!!!")             
    
    print("test18+19+20")
    try: 
        tmp2 = pyBlock.block_query("restricted.txt")
        tmp3 = pyBlock.block_query("restricted.txt")
        tmp4 = pyBlock.block_query("restricted.txt")
        if not tmp2 == 1 and not tmp3 == 1 and not tmp4 == 1: 
            print("test18+19+20 fail !!!!!!!!!!!!!!!!!!!")
            
    except Exception , ex:
        print(ex)  
        print("test18+19+20 fail !!!!!!!!!!!!!!!!!!!")      
        
if __name__ == "__main__":
    test1()