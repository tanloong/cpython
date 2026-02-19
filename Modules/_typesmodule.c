/* _types module */

#include "Python.h"
#include "pycore_descrobject.h"   // _PyMethodWrapper_Type
#include "pycore_lazyimportobject.h" // PyLazyImport_Type
#include "pycore_namespace.h"     // _PyNamespace_Type
#include "pycore_object.h"        // _PyNone_Type, _PyNotImplemented_Type
#include "pycore_unionobject.h"   // _PyUnion_Type
#include "pycore_typeobject.h"    // _PyObject_LookupSpecialMethod
#include "pycore_stackref.h"      // _PyStackRef
#include "clinic/_typesmodule.c.h"

/*[clinic input]
module _types
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=530308b1011b659d]*/

/*[clinic input]
_types.lookup_special_method

    obj: 'O'
    attr: 'O'
    /

Lookup special method name `attr` on `obj`.

Lookup method `attr` on `obj` without looking in the instance
dictionary. For methods defined in class `__dict__` or `__slots__`, it
returns the unbound function (descriptor), not a bound method. The
caller is responsible for passing the object as the first argument when
calling it:

    class A:
        def __enter__(self):
            pass

    class B:
        __slots__ = ("__enter__",)

        def __init__(self):
            def __enter__(self):
                pass
            self.__enter__ = __enter__

    a = A()
    b = B()
    enter_a = types.lookup_special_method(a, "__enter__")
    enter_b = types.lookup_special_method(b, "__enter__")

    result_a = enter_a(a)
    result_b = enter_b(b)

For other descriptors (property, etc.), it returns the result of the
descriptor's `__get__` method. Returns `None` if the method is not
found.
[clinic start generated code]*/

static PyObject *
_types_lookup_special_method_impl(PyObject *module, PyObject *obj,
                                  PyObject *attr)
/*[clinic end generated code: output=890e22cc0b8e0d34 input=fca9cb0e313a7848]*/
{
    if (!PyUnicode_Check(attr)) {
        PyErr_Format(PyExc_TypeError,
                     "attribute name must be string, not '%.200s'",
                     Py_TYPE(attr)->tp_name);
        return NULL;
    }
    _PyStackRef method_and_self[2];
    method_and_self[0] = PyStackRef_NULL;
    method_and_self[1] = PyStackRef_FromPyObjectBorrow(obj);
    int result = _PyObject_LookupSpecialMethod(attr, method_and_self);
    if (result == -1) {
        return NULL;
    }
    if (result == 0) {
        Py_RETURN_NONE;
    }
    PyObject *method = PyStackRef_AsPyObjectSteal(method_and_self[0]);
    return method;
}

static int
_types_exec(PyObject *m)
{
#define EXPORT_STATIC_TYPE(NAME, TYPE)                                   \
    do {                                                                 \
        assert(PyUnstable_IsImmortal((PyObject *)&(TYPE)));              \
        if (PyModule_AddObjectRef(m, (NAME), (PyObject *)&(TYPE)) < 0) { \
            return -1;                                                   \
        }                                                                \
    } while (0)

    EXPORT_STATIC_TYPE("AsyncGeneratorType", PyAsyncGen_Type);
    EXPORT_STATIC_TYPE("BuiltinFunctionType", PyCFunction_Type);
    // BuiltinMethodType is the same as BuiltinFunctionType
    EXPORT_STATIC_TYPE("BuiltinMethodType", PyCFunction_Type);
    EXPORT_STATIC_TYPE("CapsuleType", PyCapsule_Type);
    EXPORT_STATIC_TYPE("CellType", PyCell_Type);
    EXPORT_STATIC_TYPE("ClassMethodDescriptorType", PyClassMethodDescr_Type);
    EXPORT_STATIC_TYPE("CodeType", PyCode_Type);
    EXPORT_STATIC_TYPE("CoroutineType", PyCoro_Type);
    EXPORT_STATIC_TYPE("EllipsisType", PyEllipsis_Type);
    EXPORT_STATIC_TYPE("FrameType", PyFrame_Type);
    EXPORT_STATIC_TYPE("FrameLocalsProxyType", PyFrameLocalsProxy_Type);
    EXPORT_STATIC_TYPE("FunctionType", PyFunction_Type);
    EXPORT_STATIC_TYPE("GeneratorType", PyGen_Type);
    EXPORT_STATIC_TYPE("GenericAlias", Py_GenericAliasType);
    EXPORT_STATIC_TYPE("GetSetDescriptorType", PyGetSetDescr_Type);
    // LambdaType is the same as FunctionType
    EXPORT_STATIC_TYPE("LambdaType", PyFunction_Type);
    EXPORT_STATIC_TYPE("LazyImportType", PyLazyImport_Type);
    EXPORT_STATIC_TYPE("MappingProxyType", PyDictProxy_Type);
    EXPORT_STATIC_TYPE("MemberDescriptorType", PyMemberDescr_Type);
    EXPORT_STATIC_TYPE("MethodDescriptorType", PyMethodDescr_Type);
    EXPORT_STATIC_TYPE("MethodType", PyMethod_Type);
    EXPORT_STATIC_TYPE("MethodWrapperType", _PyMethodWrapper_Type);
    EXPORT_STATIC_TYPE("ModuleType", PyModule_Type);
    EXPORT_STATIC_TYPE("NoneType", _PyNone_Type);
    EXPORT_STATIC_TYPE("NotImplementedType", _PyNotImplemented_Type);
    EXPORT_STATIC_TYPE("SimpleNamespace", _PyNamespace_Type);
    EXPORT_STATIC_TYPE("TracebackType", PyTraceBack_Type);
    EXPORT_STATIC_TYPE("UnionType", _PyUnion_Type);
    EXPORT_STATIC_TYPE("WrapperDescriptorType", PyWrapperDescr_Type);
#undef EXPORT_STATIC_TYPE
    return 0;
}

static struct PyModuleDef_Slot _typesmodule_slots[] = {
    {Py_mod_exec, _types_exec},
    {Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
    {Py_mod_gil, Py_MOD_GIL_NOT_USED},
    {0, NULL}
};

static PyMethodDef _typesmodule_methods[] = {
    _TYPES_LOOKUP_SPECIAL_METHOD_METHODDEF
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef typesmodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "_types",
    .m_doc = "Define names for built-in types.",
    .m_size = 0,
    .m_slots = _typesmodule_slots,
    .m_methods = _typesmodule_methods,
};

PyMODINIT_FUNC
PyInit__types(void)
{
    return PyModuleDef_Init(&typesmodule);
}
