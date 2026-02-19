/*[clinic input]
preserve
[clinic start generated code]*/

#include "pycore_modsupport.h"    // _PyArg_CheckPositional()

PyDoc_STRVAR(_types_lookup_special_method__doc__,
"lookup_special_method($module, obj, attr, /)\n"
"--\n"
"\n"
"Lookup special method name `attr` on `obj`.\n"
"\n"
"Lookup method `attr` on `obj` without looking in the instance dictionary.\n"
"Returns `None` if the method is not found.");

#define _TYPES_LOOKUP_SPECIAL_METHOD_METHODDEF    \
    {"lookup_special_method", _PyCFunction_CAST(_types_lookup_special_method), METH_FASTCALL, _types_lookup_special_method__doc__},

static PyObject *
_types_lookup_special_method_impl(PyObject *module, PyObject *obj,
                                  PyObject *attr);

static PyObject *
_types_lookup_special_method(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *obj;
    PyObject *attr;

    if (!_PyArg_CheckPositional("lookup_special_method", nargs, 2, 2)) {
        goto exit;
    }
    obj = args[0];
    attr = args[1];
    return_value = _types_lookup_special_method_impl(module, obj, attr);

exit:
    return return_value;
}
/*[clinic end generated code: output=5e1740bceb7577bc input=a9049054013a1b77]*/
