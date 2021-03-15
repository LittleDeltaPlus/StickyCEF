//
// Created by lildeltaplus on 14/03/2021.
//

#ifndef STICKYCEF_UPDATE_INKY_H
#define STICKYCEF_UPDATE_INKY_H
#pragma once

#include "pyhelper.hpp"

class InkyHelper {
public:
    InkyHelper(){
        CPyInstance pyInstance;
        CPyObject sysPath = PySys_GetObject("path");
        PyList_Insert(sysPath, 0, PyUnicode_FromString("/home/pi/Downloads/StickyCEF/src"));

        CPyObject pName = PyUnicode_FromString("update_inky");
        CPyObject pModule = PyImport_Import(pName);
        if(pModule) {
            pFunc = PyObject_GetAttrString(pModule, "display_image");
        }
    }

    void CallHelperFunction(CPyObject pArgs){
        if (pFunc && PyCallable_Check(pFunc)) {
            CPyObject pValue = PyObject_CallObject(pFunc, pArgs);
            PyErr_Print();
        }
    }

private:
    CPyObject pFunc;
};

void UpdateInky(InkyHelper helper, const char* img);

#endif //STICKYCEF_UPDATE_INKY_H
