#ifndef PROJECTION_MODULE_H
#define PROJECTION_MODULE_H

#include "../util/log.hpp"

// Converts a latitude/longitude pair to x and y coordinates in the
// Universal Transverse Mercator projection.
//
// Inputs:
//     lat    Latitude of the point, in degrees.
//     lon    Longitude of the point, in degrees.
//     zone    UTM zone to be used for calculating values for x and y.
//         If zone is null, the routine will determine the appropriate zone
//         from the value of lon.
//
// Outputs:
//     x    The x coordinate (easting) of the computed point. (in meters)
//     y    The y coordinate (northing) of the computed point. (in meters)
//
// Returns:
//     The UTM zone, or -1 if x or y is null or the passed zone is invalid.
int lat_lon_to_utm(
    double lat, double lon, int const *zone, double *easting, double *northing);

// Converts x and y coordinates in the Universal Transverse Mercator
// projection to a latitude/longitude pair.
//
// Inputs:
//     x        The easting of the point, in meters.
//     y        The northing of the point, in meters.
//     zone        The UTM zone in which the point lies.
//     southhemi    Greater than zero if the point is in the south
//             hemisphere.
//
// Outputs:
//     lat    The latitude of the point, in degrees.
//     lon    The longitude of the point, in degrees.
//
// Returns:
//     Zero, or -1 if lat or lon is null.
int utm_to_lat_lon(double easting,
           double northing,
           int zone,
           int southhemi,
           double *lat,
           double *lon);


class ProjectionModule {
public:
    ProjectionModule(){
    }

    void setXY(double lat_d, double lon_d, double & x, double & y){
        double easting;
        double northing;
        lat_lon_to_utm(lat_d,lon_d, NULL, &easting, &northing);
        x = easting;
        y = northing;
        
        INFO(lat_d << " " << lon_d << " " << x << " " << y);
    }

    void setLatLon(double x, double y, double & lat_d, double & lon_d){
    }

};

#endif // GPS_FRAMEWORK_H
