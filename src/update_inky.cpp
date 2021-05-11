//
// Created by lildeltaplus on 14/03/2021.
//
#include "inc/update_inky.h"
#include <Python.h>
#include "inc/pyhelper.hpp"

using namespace std;

CPyObject StartInky(){
    Py_Initialize();

    CPyObject sysPath = PySys_GetObject("path");
    PyList_Insert(sysPath, 0, PyUnicode_FromString("/home/lildeltaplus/cef-sticky/src"));

    CPyObject pModule = PyImport_ImportModule("mock_inky");
    if(pModule){
        CPyObject pFunc = PyObject_GetAttrString(pModule, "display_image");
        Py_INCREF(pFunc);
        return pFunc;
    } else {
        return nullptr;
    }
}

void CloseInky(){
    Py_Finalize();
}

bool UpdateInky(CPyObject pFunc, const char* img, int frame){
    if (pFunc && PyCallable_Check(pFunc)) {
        CPyObject pArgs = PyTuple_Pack(2, PyByteArray_FromStringAndSize(img, 400 * 300), PyLong_FromLong(long(frame)));
        CPyObject pValue = PyObject_CallObject(pFunc, pArgs);
        if(pValue && PyObject_IsTrue(pValue)){
            PyErr_Print();
            Py_INCREF(pFunc);
            return true;
        }
        PyErr_Print();
        Py_INCREF(pFunc);
    }
    return false;
}