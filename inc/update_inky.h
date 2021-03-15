//
// Created by lildeltaplus on 14/03/2021.
//

#ifndef STICKYCEF_UPDATE_INKY_H
#define STICKYCEF_UPDATE_INKY_H
#pragma once

#include "inc/pyhelper.hpp"

void UpdateInky(CPyObject pModule ,const char* img);

CPyObject StartInky();

void CloseInky();

#endif //STICKYCEF_UPDATE_INKY_H
