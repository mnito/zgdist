/**
 * Zippy geographical distance (zgdist)
 *
 * Performs fast approximate geographical distance calculations using
 * properties of ellipsoidal geometry
 *
 *
 * Dual-licensed under the MIT License and the Unlicense:
 *
 * The Unlicense
 *
 * This is free and unencumbered software released into the public domain.
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * --
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Michael P. Nitowski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *
 * Authors:
 * Michael P. Nitowski <mike@nitow.ski> (Original Author)
 *
 *
 * This library was heavily inspired by approaches taken in:
 * https://github.com/mapbox/cheap-ruler
 * https://github.com/mapbox/cheap-ruler-cpp
 * https://github.com/Turbo87/flat-projection-rs
 *
 */

#ifndef ZGDISTF_H
#define ZGDISTF_H

#include <stddef.h>

// For converting to radians
#define ZGDIST_DEGREE M_PI / 180.0F

// Earth reference ellipsoid constants (WGS84)
// Semi major axis
#define ZGDIST_A_EARTH 6378.137F // Kilometers
// Semi minor axis
#define ZGDIST_B_EARTH 6356.752314245F // Kilometers

// Unit multipliers
#define ZGDIST_UNIT_KM 1.0F
#define ZGDIST_UNIT_M 1000.0F
#define ZGDIST_UNIT_MI 0.62137119F
#define ZGDIST_UNIT_FT 3280.84F
#define ZGDIST_UNIT_NMI 0.5399568F

// Functions
#define ZGDIST_sqrt sqrtf
#define ZGDIST_cos cosf

// Flattening
#define ZGDIST_F_EARTH (ZGDIST_A_EARTH - ZGDIST_B_EARTH) / ZGDIST_A_EARTH
// Eccentricity Squared
#define ZGDIST_E2_EARTH ZGDIST_F_EARTH * (2 - ZGDIST_F_EARTH)

typedef float ZGDIST_real;

/**
 * Container for storing projection multipliers
 */
typedef struct ZGDIST_M {
    float latm;
    float lonm;
} ZGDIST_M;

/**
 * Calculates the unit per degree difference to generate conversion multipliers
 */
ZGDIST_M ZGDIST_u_per_deg_difference(float lat, float unitm);

/**
 * With conversion multipliers, calculates the distance between points
 */
float ZGDIST_dist(
    float lat1,
    float lon1,
    float lat2,
    float lon2,
    float latm,
    float lonm
);

/**
 * Compute distance among multiple points
 */
float ZGDIST_vdist(
    float* vlat,
    float* vlon,
    size_t n,
    float latm,
    float lonm
);

#endif /* ZGDISTF_H */
