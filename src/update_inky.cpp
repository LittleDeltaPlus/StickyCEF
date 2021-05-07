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
    PyList_Insert(sysPath, 0, PyUnicode_FromString("/home/pi/Downloads/StickyCEF/src"));

    CPyObject pModule = PyImport_ImportModule("update_inky");
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

void UpdateInky(CPyObject pFunc, const char* img){
    if (pFunc && PyCallable_Check(pFunc)) {
        CPyObject pArgs = PyTuple_Pack(1, PyByteArray_FromStringAndSize(img, 400 * 300));
        CPyObject pValue = PyObject_CallObject(pFunc, pArgs);
        PyErr_Print();
        Py_INCREF(pFunc);
    }
}