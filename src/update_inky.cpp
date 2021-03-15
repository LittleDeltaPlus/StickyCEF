//
// Created by lildeltaplus on 14/03/2021.
//
#include "inc/update_inky.h"
#include <Python.h>
#include "inc/pyhelper.hpp"

using namespace std;

void UpdateInky(InkyHelper helper, const char* img){
    CPyInstance pyInstance;
    CPyObject pArgs = PyTuple_Pack(1, PyByteArray_FromStringAndSize(img, 400 * 300));
    helper.CallHelperFunction(pArgs);
}