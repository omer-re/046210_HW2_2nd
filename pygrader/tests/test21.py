import pyBlock
import os

from hwgrader.module_utils import *
from hwgrader.test_utils import *
from hwgrader.utils import ParTextTestRunner, ParTestCase, RebootableTestSuite
import fcntl
import unittest
import sys
import errno

    # תהליך לא מורשה מנסה לפתוח קובץ מוגבל
def test1():
    my_pid = os.getpid()
    fork = tfork2()
    if fork.isChild:
        print("child pid is:" + str(os.getpid()))
        fork.sync()
        print("parent pid is:" + str(os.getpid()))    
        print("test1")
        try: 
            fd = open("restricted.txt")
            if fd > 0:
                print("test1 fail !!!!!!!!!!!!!!!!!!!")
        except IOError, e:
            print(e)
            
        print("test2")
        fd = open("not_restricted.txt")
        if fd < 0:
            print("test2 fail !!!!!!!!!!!!!!!!!!!")
            
        print("test3")
        try: 
            fd = open("test3.txt")
            if fd > 0:
                print("test3 fail !!!!!!!!!!!!!!!!!!!")
        except IOError, e:
            print(e)
            
        print("test5")
        try: 
            fd = open("")
            if fd > 0:
                print("test5 fail !!!!!!!!!!!!!!!!!!!")
        except IOError, e:
            print(e)
        
        print("test8")
        tmp = pyBlock.block_query("not_restricted.txt")
        if not tmp == 0:
            print("test8 fail !!!!!!!!!!!!!!!!!!!")
        
        print("test9")
        tmp = pyBlock.block_query("restricted.txt")
        if not tmp == 1:
            print("test9 fail !!!!!!!!!!!!!!!!!!!")
        fork.exit()
    
    
    print("parent pid is:" + str(os.getpid()))    
    ret = pyBlock.block_add_process(os.getpid())
    if not ret ==1:
        print("fail !!!!!!!!!!!!!!!!!!!")
    pyBlock.block_add_file("restricted.txt")
    
    print("test4")
    fd = open("restricted.txt")
    if fd < 0:
        print("test4 fail !!!!!!!!!!!!!!!!!!!")
    
    print("test6")
    tmp = pyBlock.block_query("not_restricted.txt")
    if not tmp == 0:
        print("test6 fail !!!!!!!!!!!!!!!!!!!")
    
    print("test7")
    tmp = pyBlock.block_query("restricted.txt")
    if not tmp == 1:
        print("test7 fail !!!!!!!!!!!!!!!!!!!")
        
        
if __name__ == "__main__":
    test1()