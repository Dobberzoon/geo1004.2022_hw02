#ifndef CJDEMO_VOLUME_H
#define CJDEMO_VOLUME_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;


// Volume
const int N = 4;
void subMatrix (double (&mat)[N][N], double (&temp)[N][N], int p, int q, int n);
double determinantOfMatrix (double (&matrix)[N][N], int n);
void fillMatrix4x4 (double (&mat)[N][N], std::vector<std::vector<double>> &vertices, int n);
double volumeTetra (double &determinant);
void getCOVolumes (json &j);

// Number of floors
void getCONoFloors (json &j);

// Orientation
void collectPolygonPoints (json &j, json &g, int i, int k, std::vector<std::vector<double>> &points_set);
void collectPolygonPointsInner (json &j, json &g, int i, int k, std::vector<std::vector<double>> &points_set);
void collectPolygonPointsOuter (json &j, json &g, int i, int k, std::vector<std::vector<double>> &points_set);
std::vector<double> surfaceNormal (std::vector<std::vector<double>> &vertices);
std::string interCardinal (std::vector<double> normal);

// Area
double distance (std::vector<double> begin_v, std::vector<double> end_v);
std::vector<double> crossProduct (std::vector<double> vect_A, std::vector<double> vect_B);
double areaTriangle (std::vector<double> cross_P);
double areaPolygon (std::vector<std::vector<double>> &points_set);
void getCOAreaOrientation (json &j);


#endif //CJDEMO_VOLUME_H
