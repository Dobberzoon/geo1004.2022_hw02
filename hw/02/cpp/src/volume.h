//
// Created by Daniël Dobson on 08/03/2022.
//

#ifndef CJDEMO_VOLUME_H
#define CJDEMO_VOLUME_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

const int N = 4;

void subMatrix(double (&mat)[N][N], double (&temp)[N][N], int p, int q, int n);
double determinantOfMatrix(double (&matrix)[N][N], int n);
void readObj(std::string &file_in, std::vector<std::vector<double>> &vertices, std::vector<std::vector<int>> &face_indices);
void fillMatrix4x4(double (&mat)[N][N], std::vector<std::vector<double>> &vertices, int n);

#endif //CJDEMO_VOLUME_H