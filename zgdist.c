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

#include <math.h>

#ifndef ZGDIST_PRECISION_SINGLE
    #include "zgdist.h"
#else
    #include "zgdistf.h"
#endif

/**
 * Calculates the unit per degree difference by computing the
 * radius of curvature in the meridian and in the prime vertical
 * at a given latitude and multiplying it by a unit multiplier
 *
 * See:
 * https://en.wikipedia.org/wiki/Earth_radius#Radii_of_curvature
 * http://www2.unb.ca/gge/Pubs/LN39.pdf
 *
 */
ZGDIST_M ZGDIST_u_per_deg_difference(ZGDIST_real lat, ZGDIST_real unitm) {
    ZGDIST_M m;

    ZGDIST_real coslat = ZGDIST_cos(lat * ZGDIST_DEGREE);
    // ZGDIST_sqrt(1 - ZGDIST_E2_EARTH * sin2(lat * ZGDIST_DEGREE))
    ZGDIST_real v = ZGDIST_sqrt(1 - (ZGDIST_E2_EARTH * (1 - coslat * coslat)));

    // Meridional radius of curvature * unit multiplier (N-S)
    m.latm = ZGDIST_DEGREE * (ZGDIST_A_EARTH * (1 - ZGDIST_E2_EARTH))
                / (v * v * v) * unitm;

    // Prime vertical (transverse) radius of curvature * unit multiplier (E-W)
    m.lonm = ZGDIST_DEGREE * coslat * (ZGDIST_A_EARTH / v) * unitm;

    return m;
}

/**
 * With Euclidean plane conversion multipliers, calculates the distance
 * between points
 */
ZGDIST_real ZGDIST_dist(
    ZGDIST_real lat1,
    ZGDIST_real lon1,
    ZGDIST_real lat2,
    ZGDIST_real lon2,
    ZGDIST_real latm,
    ZGDIST_real lonm
) {
    ZGDIST_real dy = (lat2 - lat1) * latm;
    ZGDIST_real dx = (lon2 - lon1) * lonm;
    return ZGDIST_sqrt(dy * dy + dx * dx);
}

/**
 * Compute distance among multiple points
 *
 * Can usually be vectorized with optimization level of 2 or above and
 * fast math compiler flags (with possible loss in accuracy)
 */
ZGDIST_real ZGDIST_vdist(
    ZGDIST_real* vlat,
    ZGDIST_real* vlon,
    size_t n,
    ZGDIST_real latm,
    ZGDIST_real lonm
) {
    ZGDIST_real d = 0;
    for (size_t i = 0; i < (n - 1); i++) {
        d += ZGDIST_dist(
            vlat[i], vlon[i], vlat[i + 1], vlon[i + 1], latm, lonm
        );
    }
    return d;
}
