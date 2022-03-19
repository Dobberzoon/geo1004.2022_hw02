/*
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|  _____________ ______  ___  ____   ___  ___  ___  ___    __         ___  ___ |
| / ___/ __/ __ <  / _ \/ _ \/ / /  |_  |/ _ \|_  ||_  |  / / _    __/ _ \|_  ||
|/ (_ / _// /_/ / / // / // /_  _/ / __// // / __// __/_ / _ \ |/|/ / // / __/ |
|\___/___/\____/_/\___/\___/ /_/(_)____/\___/____/____(_)_//_/__,__/\___/____/ |
|                                                                              |
|                                  __       __                                 |
|                                 / /  ___ / /__                               |
|                                / _ \/ -_) / _ \                              |
|                               /_//_/\__/_/ .__/                              |
|                                         /_/                                  |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
+------------------------------------------------------------------------------+
+------------------------------------------------------------------------------+
|                                                                              |
|                                 Hugo Ledoux                                  |
|                             h.ledoux@tudelft.nl                              |
|                                  2022-02-22                                  |
|                                                                              |
+------------------------------------------------------------------------------+
*/

#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"
#include "newattributes.h"

using json = nlohmann::json;


void roofSurfaceArea (json &j);

void collectPolygonPoints (json &j, json &g, int i, int k, std::vector<std::vector<double>> &points_set) {

    for (auto i : g["boundaries"][i][k][0]) {
        std::vector<double> xyz;
        double x, y, z;
        x = (j["vertices"][i.get<int>()][0].get<int>() * j["transform"]["scale"][0].get<double>())
            + j["transform"]["translate"][0].get<double>();
        y = (j["vertices"][i.get<int>()][1].get<int>() * j["transform"]["scale"][1].get<double>())
            + j["transform"]["translate"][1].get<double>();
        z = (j["vertices"][i.get<int>()][2].get<int>() * j["transform"]["scale"][2].get<double>())
            + j["transform"]["translate"][2].get<double>();
        std::cout << "xyz: " << x << " " << y << " " << z << std::endl;
        xyz = {x, y, z};
        points_set.emplace_back(xyz);
    }
}

double areaPolygon (std::vector<std::vector<double>> &points_set) {
    /*
        areaPolygon finds the area by locally triangulating the polygon with finding its barycenter,
        then each triangle's area is calculating by the half area of parallelogram method.

        Input:  - Points of polygon (vector<vector<double>>)
        Output: - area (double)
     */

    double areaSum = 0.;
    // Face bary center
    std::vector<double> baryCenterFace = {0., 0., 0.};
    // Sum all point values
    for (auto i : points_set) {
        baryCenterFace[0] += i[0];
        baryCenterFace[1] += i[1];
        baryCenterFace[2] += i[2];
    }
    // Calculate barycenter by averaging all point values
    for (int i = 0; i < baryCenterFace.size(); i++) {
        baryCenterFace[i] = baryCenterFace[i] / points_set.size();
    }

    // Calculate area of polygon by triangulating the polygon,
    // then calculate area of each triangle and sum all corresponding areas.
    for (int e = 0; e < points_set.size(); e++) {
        std::vector<double> begin_v, end_v;
        std::vector<double> side1 = {0., 0., 0.};
        std::vector<double> side2 = {0., 0., 0.};
        if (e == (points_set.size() - 1)) {
            begin_v = {points_set[e][0], points_set[e][1], points_set[e][2]};
            end_v = {points_set[0][0], points_set[0][1], points_set[0][2]};
            std::transform(begin_v.begin(), begin_v.end(), baryCenterFace.begin(),
                           side1.begin(), std::minus<double>());

            std::transform(begin_v.begin(), begin_v.end(), end_v.begin(),
                           side2.begin(), std::minus<double>());

            std::vector<double> crossP = crossProduct(side1, side2);
            areaSum += areaTriangle(crossP);
        }
        else {
            begin_v = {points_set[e][0], points_set[e][1], points_set[e][2]};
            end_v = {points_set[e+1][0], points_set[e+1][1], points_set[e+1][2]};
            std::transform(begin_v.begin(), begin_v.end(), baryCenterFace.begin(),
                           side1.begin(), std::minus<double>());

            std::transform(begin_v.begin(), begin_v.end(), end_v.begin(),
                           side2.begin(), std::minus<double>());

            std::vector<double> crossP = crossProduct(side1, side2);
            areaSum += areaTriangle(crossP);
        }
    }
    return areaSum;
}

int main(int argc, const char * argv[]) {

    std::cout << "reading data.";
    //-- reading the file with nlohmann json: https://github.com/nlohmann/json
    // non- triangulated for area per polygon (and later also orientation
    std::ifstream input("../../data/3dbag_cleaned.city.json");
    json j;
    input >> j;
    input.close();
    std::cout << ".";

    // triangulated file for no-floors and volume
    std::ifstream input2("../../data/3dbag_cleaned_triangulated.city.json");
    json j2;
    input2 >> j2;
    input2.close();
    std::cout << ".";

    std::cout << "--->data is read\n";

    // Get volume of all CityObjects, and update the cityjson
    getCOVolumes(j2);

    // Get number of floors of all CityObjects, and update the cityjson
    getCOBuildingHeights(j2);

    roofSurfaceArea(j);


    //-- write to disk the modified city model (myfile.city.json)
    std::ofstream o("../../data/3dbag_original_upgraded+area+orientation_TEST2.json");
    o << j.dump(2) << std::endl;
    o.close();

    //-- write to disk the modified city model (myfile.city.json)
    std::ofstream o2("../../data/3dbag_triangulated+volume+floor_TEST2.json");
    o2 << j2.dump(2) << std::endl;
    o2.close();

    return 0;
}



void roofSurfaceArea(json &j) {
    for (auto &co: j["CityObjects"].items()) {
        for (auto &g: co.value()["geometry"]) {
            if (g["type"] == "Solid") {
                int sem_index_extended;
                for (int i = 0; i < g["boundaries"].size(); i++) {
                    for (int k = 0; k < g["boundaries"][i].size(); k++) {
                        int sem_index = g["semantics"]["values"][i][k];
                        sem_index_extended = g["semantics"]["surfaces"].size();
                        if (g["semantics"]["surfaces"][sem_index]["type"].get<std::string>().compare("RoofSurface") == 0) {
                            // Collect the boundary points
                            std::vector<std::vector<double>> points_set;
                            collectPolygonPoints(j, g, i, k, points_set);

                            // Calculate area of current visiting polygon
                            double area = areaPolygon(points_set);
                            // Calculate normal of current visiting polygon
                            std::vector<double> roof_normal = surfaceNormal(points_set);

                            // Write calculated values to the json
                            g["semantics"]["surfaces"][sem_index_extended]["type"] = "RoofSurface";
                            g["semantics"]["surfaces"][sem_index_extended]["area"] = area;
                            g["semantics"]["surfaces"][sem_index_extended]["orientation"] = interCardinal(roof_normal);
                            g["semantics"]["values"][i][k] = sem_index_extended;
                            sem_index_extended++;
                        }
                    }
                }
            }
        }
    }
}