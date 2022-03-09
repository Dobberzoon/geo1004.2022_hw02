//
// Created by Daniël Dobson on 08/03/2022.
//

#include "volume.h"

void subMatrix(double (&mat)[N][N], double (&temp)[N][N], int p, int q, int n) {
    int i = 0, j = 0;
    // filling the sub matrix
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            // skipping if the current row or column is not equal to the current
            // element row and column
            if (row != p && col != q) {
                temp[i][j++] = mat[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

void initMatrix4x4(double (&mat)[N][N], std::vector<std::vector<double>> &vertices, int n){
    // filling matrix from obj vertex list (will be similar to vertex list from json)

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == (n - 1)) {mat[i][j] = 1.;}
            else {mat[i][j] = vertices[i][j];}
        }
    }

}

double determinantOfMatrix(double (&matrix)[N][N], int n) {
    double determinant = 0.;
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
    }
    double temp[N][N], sign = 1;
    for (int i = 0; i < n; i++) {
        subMatrix(matrix, temp, 0, i, n);
        determinant += sign * matrix[0][i] * determinantOfMatrix(temp, n - 1);
        sign = -sign;
    }
    return determinant;
}

void readObj(std::string &file_in, std::vector<std::vector<double>> &vertices, std::vector<std::vector<int>> &face_indices) {
    /*
        readObj reads .obj file formats, and separates the vertices (x,y,z) from the faces('s indices) into
        two vectors. These two vectors can then be used for further processing.

        Input:  - (filepath to) .obj
                - two empty vectors for the vertices and face indices of types Vertex and int, respectively

        Output: - void function, stores output into passed vectors.
     */
    std::ifstream stream_in;
    stream_in.open(file_in); // open file

    if (stream_in.is_open()) {
        std::string line;

        while (getline(stream_in, line)) {
            std::istringstream iss(line);
            std::string word;
            iss >> word;

            // Extract all the vertices in the .obj, and store in vector vertices
            if (word == "v") {
                std::vector<double> coordinates;
                while (iss >> word) coordinates.push_back(std::stof(word));
                if (coordinates.size() == 3) vertices.emplace_back(coordinates);
                else vertices.emplace_back();
            }

            // Extract all the faces('s indices) in the .obj, and store in vector face_indices
            if (word == "f") {
                std::vector<int> face;
                while (iss >> word) face.emplace_back(std::stoi(word));
                face_indices.push_back(face);
            }
        }
    }
    stream_in.close();
}


