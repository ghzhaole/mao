//
// Created by leo on 2023/3/1.
//
#include "test_struct.h"

REGISTER_CLASS(parent)
REGISTER_CLASS_FIELD(parent, i, int)
REGISTER_CLASS_FIELD(parent, k, int)

REGISTER_CLASS(point)
REGISTER_CLASS_FIELD(point, x, float)
REGISTER_CLASS_FIELD(point, y, float)

REGISTER_CLASS(child, parent)
REGISTER_CLASS_FIELD(child, ps, vector, point)
REGISTER_CLASS_FIELD(child, vi, vector, int)
REGISTER_CLASS_FIELD(child, mi, map, int)