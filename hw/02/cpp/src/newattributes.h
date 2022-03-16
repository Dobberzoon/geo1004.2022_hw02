#ifndef CJDEMO_VOLUME_H
#define CJDEMO_VOLUME_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;

const int N = 4;

void subMatrix(double (&mat)[N][N], double (&temp)[N][N], int p, int q, int n);
double determinantOfMatrix(double (&matrix)[N][N], int n);
void readObj(std::string &file_in, std::vector<std::vector<double>> &vertices, std::vector<std::vector<int>> &face_indices);
void fillMatrix4x4(double (&mat)[N][N], std::vector<std::vector<double>> &vertices, int n);
double volumeTetra(double &determinant);
double volumeObject(std::vector<double> &outside_point, std::vector<std::vector<double>> &vertices,
                    std::vector<std::vector<int>> &face_indices);
void getCOVolumes(json &j);
void getCOBuildingHeights(json &j);

#endif //CJDEMO_VOLUME_H