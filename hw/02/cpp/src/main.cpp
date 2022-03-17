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




int   get_no_roof_surfaces(json &j);
void  list_all_vertices(json &j);
void  visit_roofsurfaces(json &j);
void store_all_vertices(json &j, std::vector<std::vector<double>> &all_vertices);
void roofSurfaceArea(json &j);

double distance (std::vector<double> begin_v, std::vector<double> end_v) {
    std::vector<double> side = {0., 0., 0.};
    std::transform(begin_v.begin(), begin_v.end(), end_v.begin(),
                                              side.begin(), std::minus<double>());

    double result = sqrt((side[0]*side[0]) + (side[1] * side[1]) + (side[2] * side[2]));
    return result;
}

double heron (std::vector<std::vector<double>> triangle) {
    double area, sp, a, b, c;
    a = distance(triangle[0], triangle[1]);
    b = distance(triangle[1], triangle[2]);
    c = distance(triangle[2], triangle[0]);

    sp = (a+b+c)/2;

    area = sqrt(sp*(sp-a)*(sp-b)*(sp-c));
    return area;
}


int main(int argc, const char * argv[]) {

    //-- reading the file with nlohmann json: https://github.com/nlohmann/json
    // non- triangulated for area per polygon (and later also orientation
    std::ifstream input("../../data/3dbag_cleaned.city.json");
    json j;
    input >> j;
    input.close();

    // triangulated file for no-floors and volume
    std::ifstream input2("../../data/3dbag_cleaned_triangulated.city.json");
    json j2;
    input2 >> j2;
    input2.close();

    //-- get total number of RoofSurface in the file
    int noroofsurfaces = get_no_roof_surfaces(j);
    std::cout << "Total RoofSurface: " << noroofsurfaces << std::endl;

    //list_all_vertices(j);

    //visit_roofsurfaces(j);

    //-- print out the number of Buildings in the file
    int nobuildings = 0;
    for (auto& co : j["CityObjects"]) {
        if (co["type"] == "Building") {
          nobuildings += 1;
        }
    }
    std::cout << "There are " << nobuildings << " Buildings in the file" << std::endl;

    //-- print out the number of vertices in the file
    std::cout << "Number of vertices " << j["vertices"].size() << std::endl;


    //std::string file_in = "../../data/cube_triangulated_copy.obj";

    // ## Read OBJ file (FOR TESTING PURPOSES) ##
    // The vertices and faces are read and stored into vectors.

    //std::vector<std::vector<double>> vertices_one_building;
    //std::vector<std::vector<int>> face_indices_one_building;
    //readObj(file_in, vertices_one_building, face_indices_one_building);


    // ## Calculate volume of object (works on .obj) ##
    //std::vector<double> outside_point_same_vertex = {1., 1., -1.};
    //double volume_sum = volumeObject(outside_point_same_vertex, vertices_one_building, face_indices_one_building);

    //std::cout << "size of vertices_one_building: " << vertices_one_building.size() << "\n";
    //std::cout << "all the vertices_one_building of: " << file_in << "\n";

    //std::cout << "face_indices_one_building.size(): " << face_indices_one_building.size() << std::endl;
//    std::cout << "Volume of cube with outside point on same vertex: " << volume_sum << std::endl;


    // Get volume of all CityObjects, and update the cityjson
    getCOVolumes(j2);

    // Get number of floors of all CityObjects, and update the cityjson
    getCOBuildingHeights(j2);

    roofSurfaceArea(j);

    std::vector<double> a, b;
    a = {1.5, 1.5, 1.5};
    b = {2.5, 2.5, 2.5};
    std::cout << "DISTANCE BRO: " << distance(a, b) << std::endl;

    std::vector<double> vecA = { 3., -5., 4. };
    std::vector<double> vecB = { 2.,  6., 5. };
    std::vector<double> crossP = crossProduct(vecA, vecB);

    std::cout << "CROSSP BRO: " << std::endl;

    for (auto i : crossP ) {std::cout << i << " ";}

    std::cout << "MAGNITUDE BRO: " << areaTriangle(crossP) << std::endl;
    std::cout << std::endl;


    //-- write to disk the modified city model (myfile.city.json)
    std::ofstream o("../../data/3dbag_original_upgraded+area.json");
    o << j.dump(2) << std::endl;
    o.close();

    //-- write to disk the modified city model (myfile.city.json)
    std::ofstream o2("../../data/3dbag_triangulated+volume+floor.json");
    o2 << j2.dump(2) << std::endl;
    o2.close();

    return 0;
}


// Visit every 'RoofSurface' in the CityJSON model and output its geometry (the arrays of indices)
// Useful to learn to visit the geometry boundaries and at the same time check their semantics.

void visit_roofsurfaces(json &j) {
    for (auto& co : j["CityObjects"].items()) {
        for (auto& g : co.value()["geometry"]) {
          if (g["type"] == "Solid") {
            for (int i = 0; i < g["boundaries"].size(); i++) {
              for (int j = 0; j < g["boundaries"][i].size(); j++) {
                int sem_index = g["semantics"]["values"][i][j];
                if (g["semantics"]["surfaces"][sem_index]["type"].get<std::string>().compare("RoofSurface") == 0) {
                  std::cout << "RoofSurface: " << g["boundaries"][i][j] << std::endl;
                }
              }
            }
          }
        }
    }
}


// Returns the number of 'RooSurface' in the CityJSON model
int get_no_roof_surfaces(json &j) {
    int total = 0;
    for (auto& co : j["CityObjects"].items()) {
        for (auto& g : co.value()["geometry"]) {
          if (g["type"] == "Solid") {
            for (auto& shell : g["semantics"]["values"]) {
              for (auto& s : shell) {
                if (g["semantics"]["surfaces"][s.get<int>()]["type"].get<std::string>().compare("RoofSurface") == 0) {
                  total += 1;
                }
              }
            }
          }
        }
    }
    return total;
}


// CityJSON files have their vertices compressed: https://www.cityjson.org/specs/1.1.1/#transform-object
// this function visits all the surfaces and print the (x,y,z) coordinates of each vertex encountered
void list_all_vertices(json& j) {
    for (auto& co : j["CityObjects"].items()) {
        std::cout << "= CityObject: " << co.key() << std::endl;
        for (auto& g : co.value()["geometry"]) {
          if (g["type"] == "Solid") {
            for (auto& shell : g["boundaries"]) {
              for (auto& surface : shell) {
                for (auto& ring : surface) {
                  std::cout << "---" << std::endl;
                  for (auto& v : ring) {
                    std::vector<int> vi = j["vertices"][v.get<int>()];
                    double x = (vi[0] * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>();
                    double y = (vi[1] * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>();
                    double z = (vi[2] * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>();
                    std::cout << std::setprecision(2) << std::fixed << v << " (" << x << ", " << y << ", " << z << ")" << std::endl;
                  }
                }
              }
            }
          }
        }
    }
}



void roofSurfaceArea(json &j) {
    for (auto &co: j["CityObjects"].items()) {
        for (auto &g: co.value()["geometry"]) {
            if (g["type"] == "Solid") {
                double area = 0.;
                double area2 = 0.;
                int sem_index_extended;
                for (int i = 0; i < g["boundaries"].size(); i++) {
                    for (int k = 0; k < g["boundaries"][i].size(); k++) {
                        int sem_index = g["semantics"]["values"][i][k];
                        sem_index_extended = g["semantics"]["surfaces"].size();

                        if (g["semantics"]["surfaces"][sem_index]["type"].get<std::string>().compare("RoofSurface") == 0) {

                            std::cout << "RoofSurface: " << g["boundaries"][i][k] << std::endl;

                            std::vector<std::vector<double>> points_set;

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
                            std::vector<double> baryCenterFace = {0., 0., 0.};
                            for (auto i : points_set) {
                                baryCenterFace[0] += i[0];
                                baryCenterFace[1] += i[1];
                                baryCenterFace[2] += i[2];
                            }

                        std::cout << "baryCenterFace before fraction: " << std::endl;
                            for (auto i : baryCenterFace) {
                                std::cout << i << " ";
                            }
                            std::cout << std::endl;

                            for (int i = 0; i < baryCenterFace.size(); i++) {
                                baryCenterFace[i] = baryCenterFace[i] / points_set.size();
                            }

                            std::cout << "baryCenterFace after fraction: " << std::endl;
                            for (auto i : baryCenterFace) {
                                std::cout << i << " ";
                            }
                            std::cout << std::endl;
                            std::cout << "points_set.size(): " << points_set.size() << std::endl;

                            std::cout << "check all them vertices: " << std::endl;
                            std::cout << "last vertex: " << g["boundaries"][i][k][0][g["boundaries"][i][k][0].size() - 1] << std::endl;
                            for (auto index : g["boundaries"][i][k][0]) {
                                if (index == g["boundaries"][i][k][0][g["boundaries"][i][k][0].size() - 1]) {
                                    std::cout << "found it: " << index << std::endl;
                                }

                            }
                            std::cout << std::endl;

                            double areaSum = 0.;
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
                            std::cout << "areaSum: " << areaSum << std::endl;

                            g["semantics"]["surfaces"][sem_index_extended]["type"] = "RoofSurface";
                            g["semantics"]["surfaces"][sem_index_extended]["area"] = areaSum;
                            g["semantics"]["values"][i][k] = sem_index_extended;
                            sem_index_extended++;

                            /*for (int e = 0; e < g["boundaries"][i][k][0].size(); e++) {
                                if (g["boundaries"][i][k][0][e] == g["boundaries"][i][k][0][g["boundaries"][i][k][0].size() - 1]) {
//                                    std::cout << "found it: " << g["boundaries"][i][k][0][e] << std::endl;
                                }
                                else {

                                }
                            }*/

/*                            std::vector<std::vector<double>> triangle;
                            std::vector<double> a, b, c;
                            int ax, ay, az, bx, by, bz, cx, cy, cz;
                            double AX, AY, AZ, BX, BY, BZ, CX, CY, CZ;
                            ax = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][0].get<int>();
                            ay = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][1].get<int>();
                            az = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][2].get<int>();

                            bx = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][0].get<int>();
                            by = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][1].get<int>();
                            bz = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][2].get<int>();

                            cx = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][0].get<int>();
                            cy = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][1].get<int>();
                            cz = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][2].get<int>();

                            AX = (ax * j["transform"]["scale"][0].get<double>()) +
                                 j["transform"]["translate"][0].get<double>();
                            AY = (ay * j["transform"]["scale"][1].get<double>()) +
                                 j["transform"]["translate"][1].get<double>();
                            AZ = (az * j["transform"]["scale"][2].get<double>()) +
                                 j["transform"]["translate"][2].get<double>();


                            BX = (bx * j["transform"]["scale"][0].get<double>()) +
                                 j["transform"]["translate"][0].get<double>();
                            BY = (by * j["transform"]["scale"][1].get<double>()) +
                                 j["transform"]["translate"][1].get<double>();
                            BZ = (bz * j["transform"]["scale"][2].get<double>()) +
                                 j["transform"]["translate"][2].get<double>();


                            CX = (cx * j["transform"]["scale"][0].get<double>()) +
                                 j["transform"]["translate"][0].get<double>();
                            CY = (cy * j["transform"]["scale"][1].get<double>()) +
                                 j["transform"]["translate"][1].get<double>();
                            CZ = (cz * j["transform"]["scale"][2].get<double>()) +
                                 j["transform"]["translate"][2].get<double>();


                            a = {AX, AY, AZ};
                            triangle.emplace_back(a);
                            b = {BX, BY, BZ};
                            triangle.emplace_back(b);
                            c = {CX, CY, CZ};
                            triangle.emplace_back(c);
                            area += heron(triangle); // old

                            std::vector<double> side1 = {0., 0., 0.};
                            std::vector<double> side2 = {0., 0., 0.};
                            std::transform(a.begin(), a.end(), c.begin(),
                                           side1.begin(), std::minus<double>());

                            std::transform(a.begin(), a.end(), b.begin(),
                                           side2.begin(), std::minus<double>());

                            std::vector<double> crossP = crossProduct(side1, side2);
                            area2 += areaTriangle(crossP);
                            std::cout << "CROSSP: ";
                            for (auto i : crossP) {std::cout << i << " ";}
                            std::cout << std::endl;
                            //std::cout << "area triangle: " << area << std::endl;
                            g["semantics"]["surfaces"][sem_index_extended]["type"] = "RoofSurface";
                            g["semantics"]["surfaces"][sem_index_extended]["area"] = area;
                            g["semantics"]["values"][i][k] = sem_index_extended;
                            sem_index_extended++;*/
                        }
                    }
                }
//                std::cout << "Area of NEW ROOF BRO: " << area << std::endl;
//                std::cout << "Area2 of NEW ROOF BRO: " << area2 << std::endl;
//                std::cout << co.key() << std::endl;
            }
        }

    }
}

/*
void store_all_vertices(json& j, std::vector<std::vector<double>>& all_vertices) {
for (auto& co : j["CityObjects"].items()) {
    //std::cout << "= CityObject: " << co.key() << std::endl;
    for (auto& g : co.value()["geometry"]) {
        if (g["type"] == "Solid") {
            for (auto& shell : g["boundaries"]) {
                for (auto& surface : shell) {
                    for (auto& ring : surface) {
                        //std::cout << "---" << std::endl;
                        for (auto& v : ring) {
                            std::vector<int> vi = j["vertices"][v.get<int>()];
                            std::vector<double> vr;
                            double x = (vi[0] * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>();
                            double y = (vi[1] * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>();
                            double z = (vi[2] * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>();
                            //std::cout << std::setprecision(2) << std::fixed << v << " (" << x << ", " << y << ", " << z << ")" << std::endl;
                            //vr.emplace_back(x); vr.emplace_back(y); vr.emplace_back(z);
                            //all_vertices.emplace_back(vr);
                        }
                    }
                }
            }
        }
    }
}

// here under is the start of the code to decode all the vertices
for (auto& i : j["vertices"]) {
    std::vector<double> vec;
    //std::cout << "i: " << i << "\n";
    //std::cout << "i: " << i[0] << " " << i[1] << " " << i[2] << "\n";
    for (auto& j : i) {
        //std::cout << "indices: " << j << " ";
    }
    //std::cout << "\n";
}
}
*/

/*
// This loop traverses all CityObjects and calculates each volume, writing it as an attribute
std::string unit_volume = "m^3";
for (auto& co : j["CityObjects"].items()) {

int count_children2 = 0;
if (co.value()["type"] == "Building") {
    double volume = 0.;

    for (auto& i : co.value()["children"]) {
        count_children2++;

        for (auto& g : j["CityObjects"][i.get<std::string>()]["geometry"]) {
            int count_loop = 0;
            for (int i = 0; i < g["boundaries"].size(); i++) {
                for (int k = 0; k < g["boundaries"][i].size(); k++) {
                    std::vector<std::vector<double>> vertices_current;
                    count_loop++;
                    std::vector<double> vertex_cur1; std::vector<double> vertex_cur2; std::vector<double> vertex_cur3;

                    int x1, x2, x3, y1, y2, y3, z1, z2, z3;
                    x1 = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][0].get<int>();
                    y1 = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][1].get<int>();
                    z1 = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][2].get<int>();
                    x2 = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][0].get<int>();
                    y2 = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][1].get<int>();
                    z2 = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][2].get<int>();
                    x3 = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][0].get<int>();
                    y3 = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][1].get<int>();
                    z3 = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][2].get<int>();

                    double X1, X2, X3, Y1, Y2, Y3, Z1, Z2, Z3;
                    X1 = (x1 * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>();
                    Y1 = (y1 * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>();
                    Z1 = (z1 * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>();
                    X2 = (x2 * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>();
                    Y2 = (y2 * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>();
                    Z2 = (z2 * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>();
                    X3 = (x3 * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>();
                    Y3 = (y3 * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>();
                    Z3 = (z3 * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>();

                    vertex_cur1 = {X1, Y1, Z1};
                    vertices_current.emplace_back(vertex_cur1);
                    vertex_cur2 = {X2, Y2, Z2};
                    vertices_current.emplace_back(vertex_cur2);
                    vertex_cur3 = {X3, Y3, Z3};
                    vertices_current.emplace_back(vertex_cur3);
                    vertices_current.emplace_back(outside_point);

                    double mat_tetra_current[N][N];
                    fillMatrix4x4(mat_tetra_current, vertices_current, N);
                    double determinant_tetra_current = determinantOfMatrix(mat_tetra_current, N);
                    volume += volumeTetra(determinant_tetra_current);
                }
            }
        }
    }
    // Write value as an attribute to cityjson
    co.value()["attributes"]["volume"] = std::to_string(volume) + unit_volume;
}
}
*/