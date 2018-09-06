# zgdist - Zippy Geographical Distance

This library performs fast approximate geographical distance calculations using
properties of ellipsoidal geometry.

### Use

```c
// Initial latitude or general area of calculation
double lat0 = 39.952383;

// Get multipliers based on latitude and unit multiplier
ZGDIST_M m = ZGDIST_u_per_deg_difference(lat0, ZGDIST_UNIT_KM);

// Calculate approximate distance between two geographical points
double distance = ZGDIST_dist(
    39.952383, -75.1657937, 39.9474931, -75.161499, m.latm, m.lonm
);
printf("%.4f km\n", distance); // 0.6553 km

// Calculate approximate distance among n geographical points
double lats[3] = {39.952383, 39.9589466, 39.9619273};
double lons[3] = {-75.1657937, -75.1900651, -75.176152};
double total_distance = ZGDIST_vdist(lats, lons, 3, m.latm, m.lonm);
printf("%.4f km\n", total_distance); // 3.4325 km
```

### Compilation

#### Double precision
```shell
cc -Wall -pedantic -O3 -c zgdist.c -o zgdist.o
```

#### Single precision
```shell
cc -Wall -pedantic -O3 -DZGDIST_PRECISION_SINGLE -c zgdist.c -o zgdistf.o
```

## License

Dual licensed under the MIT License and Unlicense. See LICENSE and UNLICENSE
files for full text of these licenses.

## Original Author

Michael P. Nitowski <mike@nitow.ski>

## Related

This library was inspired by approaches taken in:
 * https://github.com/mapbox/cheap-ruler
 * https://github.com/mapbox/cheap-ruler-cpp
 * https://github.com/Turbo87/flat-projection-rs
