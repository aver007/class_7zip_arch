#include "class7zarchiter.h"
#include "class7zarch.h"

/////////////////////////////////////////////////////////////
// python methods


void
Class7zArch_dealloc(CustomObject* self)
{
	
	if (self->arch != NULL)
	{
		delete self->arch;
		//std::wcout << "deleted arch object\n" << std::flush;
	}

	Py_TYPE(self)->tp_free((PyObject*)self);
	std::wcout << "==Class7zArch_dealloc === " << self << "\n" << std::flush;
}


PyObject*
Class7zArch_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	CustomObject* self;
	self = (CustomObject*)type->tp_alloc(type, 0);
	if (self != NULL) {
		
	}
	std::wcout << "===Class7zArch_new ==== " << self << "\n" << std::flush;

	return (PyObject*)self;
}


int
Class7zArch_init(CustomObject* self, PyObject* args, PyObject* kwds)
{
	char* pointer_from_python;
	Py_ssize_t data_len;
	
	if (!PyArg_ParseTuple(args, "y#", &pointer_from_python, &data_len))  /* convert Python -> C */
		return -1;

	try
	{
		if (self->arch != NULL)
		{
			delete self->arch;
		}
		self->arch = new Archive(pointer_from_python, data_len);
		if (self->arch == NULL)
			return -1;
	}
	catch (...)
	{
		PyErr_SetString(PyExc_ValueError, "Can't open archive");
		return -1;
	}

	return 0;
}

PyObject*
Class7zArch_get_iter(PyObject* self)
{
	CustomIteratorObject* itr{ PyObject_New(CustomIteratorObject, &Class7zArchIteratorType) };
	if (!itr)
	{
		return NULL;
	}

	itr->class_7z_arch_object = (PyObject*)self;
	itr->iter_num = 0;

	Py_INCREF(self);

	std::wcout << "==Iterator_MAKE === " << itr << "\n" << std::flush;
	return (PyObject*)itr;
}


/////////////////////////////////////////////////////////////
// class methods
PyObject*
Class7zArch_files_in_arch(CustomObject* self, PyObject* Py_UNUSED(ignored))
{
	if (self->arch == NULL)
		//PyErr_SetString(PyExc_AttributeError, "ZLP");
		return NULL;

	Py_ssize_t n = self->arch->files_in_arch();

	return Py_BuildValue("n", n);        /* convert C -> Python */
}


PyObject*
Class7zArch_file_size(CustomObject * self, PyObject * args)
{
	Py_ssize_t file_num;

	if (self->arch == NULL)
		//PyErr_SetString(PyExc_AttributeError, "ZLP");
		return NULL;

	if (!PyArg_ParseTuple(args, "n", &file_num))  /* convert Python -> C */
		return NULL;                              /* null=raise exception */

	Py_ssize_t n = self->arch->filesize(file_num);

	return Py_BuildValue("n", n);        /* convert C -> Python */
}


PyObject*
Class7zArch_file_path(CustomObject * self, PyObject * args)
{
	Py_ssize_t file_num;

	if (self->arch == NULL)
		//PyErr_SetString(PyExc_AttributeError, "ZLP");
		return NULL;

	if (!PyArg_ParseTuple(args, "n", &file_num))  /* convert Python -> C */
		return NULL;                              /* null=raise exception */

	try
	{

		std::wstring path = self->arch->filepath(file_num);
		std::string path_str{ wstring_to_utf8(path) };

		return Py_BuildValue("s#", path_str.data(), path_str.length());        /* convert C -> Python */
	}catch (...)
	{
		PyErr_SetString(PyExc_LookupError, "Can't get filename");
		return NULL;
	}
}

PyObject*
Class7zArch_extract(CustomObject * self, PyObject * args)
{
	Py_ssize_t file_num;

	if (self->arch == NULL)
		//PyErr_SetString(PyExc_AttributeError, "ZLP");
		return NULL;

	if (!PyArg_ParseTuple(args, "n", &file_num))  /* convert Python -> C */
		return NULL;                              /* null=raise exception */

	bytes_vector file_data = std::move(self->arch->extract_filedata(file_num));

	return Py_BuildValue("y#", file_data.data(), file_data.size());
}

/////////////////////////////////////////////////////////////
// register methods
PyMethodDef Class7zArch_methods[] = {
	{"files_in_arch", (PyCFunction)Class7zArch_files_in_arch, METH_NOARGS, "Return number of files in archive"},
	{"file_size", (PyCFunction)Class7zArch_file_size, METH_VARARGS, "Return file size by number"},
	{"file_path", (PyCFunction)Class7zArch_file_path, METH_VARARGS, "Return file path by number"},
	{"extract", (PyCFunction)Class7zArch_extract, METH_VARARGS, "Extract file data by number"},
	{NULL}  /* Sentinel */
};


/////////////////////////////////////////////////////////////
// register class type
PyTypeObject Class7zArchType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"class_7z_arch.Class7zArch",	//.tp_name =
	sizeof(CustomObject), 0,		// tp_basicsize, tp_itemsize
	(destructor)Class7zArch_dealloc,		// .tp_dealloc=

	0, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,

	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,				//.tp_flags = 
	"7z archive objects",			//.tp_doc = 

	NULL, NULL, NULL, NULL,

	Class7zArch_get_iter,                  //.tp_iter=   __iter__() method 

	NULL, // Class7zArch_iternext,             //.tp_iternext=    next() method    

	Class7zArch_methods,		//.tp_methods=
	NULL, //Class7zArch_members,       //.tp_members=

	NULL, NULL, NULL, NULL, NULL, NULL,

	(initproc)Class7zArch_init, //.tp_init=

	NULL,

	Class7zArch_new,				//.tp_new = 
	// 
};