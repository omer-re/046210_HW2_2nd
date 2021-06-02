To compile the python extension put your 'highscore_api.h' header file in this
folder and type the following command in the terminal:

python setup.py build_ext -b .

If the compilation succeeds a new file will be created: 'pyBlock.so'.
This extension presents four functions that call your new system calls:
1) block_add_file
2) block_clear
2) block_query
3) block_add_process

You can use this functions in a python script or directly from the python
interpreter, type 'python' in the terminal and then the following commands:

>>>import pyBlock
>>>import os
>>>pyBlock.block_add_file('/some/file')

The syntax of the command can be found by typing the following in the python
interpreter:

>>>import pyBlock
>>>help(pyBlock.block_add_file)

You can also use the ipython interpreter (you can find the rpm package in the
course website). After running ipython (type 'ipython' in the terminal) do:

[1] import pyBlock
[2] pyBlock.block_add_file?
