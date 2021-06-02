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
    print("parent pid is:" + str(os.getpid()))    
    ret = pyBlock.block_add_process(os.getpid())
    if not ret ==1:
        print("failed!!!!!!!!!!!!!!!")
    fork = tfork2()
    if fork.isChild:
        print("child pid is:" + str(os.getpid()))
        fork.sync()
        print("test30")
        try: 
            open("restricted.txt")
            tmp = pyBlock.block_query("restricted.txt")
            if not tmp==1:
                 print("test30 fail !!!!!!!!!!!!!!!!!!!")
        except Exception , ex:
            print(ex)
            print("test30 fail !!!!!!!!!!!!!!!!!!!")
        try: 
            ret = pyBlock.block_add_process(13234322)
            if not ret ==2:
                print("test31 fail!!!!!!!!!!!!!!!") 
            print("test31 fail !!!!!!!!!!!!!!!!!!!ed")
        except Exception , ex:
            print(ex) 
        fork.exit()
  

    

        

    pyBlock.block_add_file("restricted.txt")
    
             
        
        
if __name__ == "__main__":
    test1()