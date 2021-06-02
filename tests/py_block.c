#include <Python.h>
#include "block_api.h"


static PyObject *
posix_error(void)
{
  return PyErr_SetFromErrno(PyExc_OSError);
}


static PyObject *
py_block_add_file(PyObject *self, PyObject *args)
{
  const char *filename;
  int status;
  
  if (!PyArg_ParseTuple(args, "z", &filename))
    return NULL;
  
  status = block_add_file(filename);
  
  if (status < 0)
    return posix_error();
  
  Py_INCREF(Py_None);
  return Py_None;
}


static PyObject *
py_block_clear(PyObject *self, PyObject *args)
{
  int status;

  status = block_clear();
  
  if (status < 0) {
    return posix_error();
  }
  
  Py_INCREF(Py_None);
  return Py_None;
}


static PyObject *
py_block_query(PyObject *self, PyObject *args)
{
  const char *filename;
  int status;

  if (!PyArg_ParseTuple(args, "z", &filename))
    return NULL;
  
  status = block_query(filename);
  
  if (status < 0) {
    return posix_error();
  }

  return Py_BuildValue("i", status);
}


static PyObject * py_block_add_process(PyObject *self, PyObject *args)
{
  int pid;
  int status;
  
  if (!PyArg_ParseTuple(args, "i", &pid))
    return NULL;
  
  status = block_add_process(pid);
  if (status < 0) {
    return posix_error();
  }
  
  return Py_BuildValue("i", status);
}


static PyMethodDef msgMethods[] = {
  {"block_add_file",  py_block_add_file, METH_VARARGS,
   "Add a file to the limited files list.\nExample:\nblock_add_file('/some/file')"},
  {"block_clear",  py_block_clear, METH_VARARGS,
   "Clears the limited files list.\nExample:\nblock_clear()"},
  {"block_query",  py_block_query, METH_VARARGS,
   "Check if a given file is limited.\nExample:\nblock_query('/some/file'). Returns 1 if file is limited, 0 otherwise."},
  {"block_add_process",  py_block_add_process, METH_VARARGS,
   "Mark a process as privileged.\nExample:\nblock_add_process(123). Returns the current number of privileged processes."},
  {NULL, NULL, 0, NULL} 
};


void
initpyBlock(void)
{
  (void) Py_InitModule("pyBlock", msgMethods);
}