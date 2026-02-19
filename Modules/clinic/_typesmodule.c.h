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
"Lookup method `attr` on `obj` without looking in the instance\n"
"dictionary. For methods defined in class `__dict__` or `__slots__`, it\n"
"returns the unbound function (descriptor), not a bound method. The\n"
"caller is responsible for passing the object as the first argument when\n"
"calling it:\n"
"\n"
">>> class A:\n"
"...    def __enter__(self):\n"
"...        return \"A.__enter__\"\n"
"...\n"
">>> class B:\n"
"...    __slots__ = (\"__enter__\",)\n"
"...    def __init__(self):\n"
"...        def __enter__(self):\n"
"...            return \"B.__enter__\"\n"
"...        self.__enter__ = __enter__\n"
"...\n"
">>> a = A()\n"
">>> b = B()\n"
">>> enter_a = types.lookup_special_method(a, \"__enter__\")\n"
">>> enter_b = types.lookup_special_method(b, \"__enter__\")\n"
">>> enter_a(a)\n"
"\'A.__enter__\'\n"
">>> enter_b(b)\n"
"\'B.__enter__\'\n"
"\n"
"For other descriptors (property, etc.), it returns the result of the\n"
"descriptor\'s `__get__` method. Returns `None` if the method is not\n"
"found.");

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
/*[clinic end generated code: output=664edfaf350f71d0 input=a9049054013a1b77]*/
