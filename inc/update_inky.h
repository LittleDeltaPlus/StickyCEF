//
// Created by lildeltaplus on 14/03/2021.
//

#ifndef STICKYCEF_UPDATE_INKY_H
#define STICKYCEF_UPDATE_INKY_H
#pragma once


#include "inc/pyhelper.hpp"

bool UpdateInky(CPyObject pFunc ,const char* img, int frame);

CPyObject StartInky();

void CloseInky();

#endif //STICKYCEF_UPDATE_INKY_H
