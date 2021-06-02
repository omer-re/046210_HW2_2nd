#!/usr/bin/python

import os
import errno
import pyBlock

def test1():
	"""simple test to invoke the syscalls with no error."""

        pyBlock.block_add_process(os.getpid())
	pyBlock.block_add_file("/some/file")
	is_limited = pyBlock.block_query("/another_file")
        pyBlock.block_clear()


if __name__ == "__main__":
    test1()
    print("TEST DONE")

