//
// Created by lildeltaplus on 14/03/2021.
//
#include "inc/update_inky.h"
#include <Python.h>
#include "inc/pyhelper.hpp"

using namespace std;

void UpdateInky(const char* img){
    CPyInstance pyInstance;

    CPyObject sysPath = PySys_GetObject("path");
    PyList_Insert(sysPath, 0, PyUnicode_FromString("/home/pi/Downloads/StickyCef/src"));

    CPyObject pName = PyUnicode_FromString("update_inky");
    CPyObject pModule = PyImport_Import(pName);

    if(pModule) {
        CPyObject pFunc = PyObject_GetAttrString(pModule, "display_image");
        if (pFunc && PyCallable_Check(pFunc)) {
            // PyObject pData = PyLong_FromLong(50);
            CPyObject pArgs = PyTuple_Pack(1, PyByteArray_FromStringAndSize(img, 400 * 300));
            CPyObject pValue = PyObject_CallObject(pFunc, pArgs);
            PyErr_Print();
        }
    }
}