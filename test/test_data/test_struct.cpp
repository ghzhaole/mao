//
// Created by leo on 2023/3/1.
//
#include "test_struct.h"

REGISTER_CLASS(parent)
REGISTER_CLASS_FIELD(parent, i, int)

REGISTER_CLASS(point)
REGISTER_CLASS_FIELD(point, x, float)
REGISTER_CLASS_FIELD(point, y, float)

REGISTER_CLASS_WITH_INHERIT(child, parent)
REGISTER_CLASS_FIELD_1(child, points, vector, int)
REGISTER_CLASS_FIELD_1(child, pointsm, map, string)
