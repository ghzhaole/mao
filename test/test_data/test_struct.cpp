//
// Created by leo on 2023/3/1.
//
#include "test_struct.h"

REGISTER_CLASS(parent)
REGISTER_CLASS_FIELD(parent, i, int)

REGISTER_CLASS(point)
REGISTER_CLASS_FIELD(point, x, float)
REGISTER_CLASS_FIELD(point, y, float)

REGISTER_CLASS(pointTable)
REGISTER_CLASS_FIELD_ARRAY(pointTable, points, vector, point)

REGISTER_CLASS_WITH_INHERIT(child, parent)

REGISTER_CLASS_FIELD(child, j, int)
REGISTER_CLASS_FIELD(child, c, int)
REGISTER_CLASS_FIELD(child, k, float)
REGISTER_CLASS_FIELD(child, l, double)
REGISTER_CLASS_FIELD(child, b, bool)
REGISTER_CLASS_FIELD(child, s, string)
REGISTER_CLASS_FIELD_ARRAY(child, points, vector, point)
