#include "newattributes.h"

// Volume
void subMatrix(double (&mat)[N][N], double (&temp)[N][N], int p, int q, int n) {
    /*
        Fills a submatrix, a sub-process of function determinantOfMatrix(...).
     */
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

double determinantOfMatrix(double (&matrix)[N][N], int n) {
    /*
        Calculates the determinant of a matrix of size n.

        Input:  - Empty matrix [N][N]
                - n (size)
        Output: - Determinant (double)
     */
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

void fillMatrix4x4(double (&mat)[N][N], std::vector<std::vector<double>> &vertices, int n){
    // filling matrix from obj vertex list (will be similar to vertex list from json)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == (n - 1)) {mat[i][j] = 1.;}
            else {mat[i][j] = vertices[i][j];}
        }
    }
}

double volumeTetra(double &determinant) {
    /*
        VolumeTetra is a tetrahedron (3-simplex) specific function to calculate its volume.
        Input:  - determinant (double)
        Output: - volume (double)
     */
    double result = (1./6.) * determinant;
    return result;
}

void getCOVolumes(json &j) {
    /*
        getVolumeAllObjects calculates the volume of each Building (CityObject) in a given cityjson file directly,
        and writes it as the attribute "volume" to each CityObject in m^3. This function takes an outside point and
        triangulated surfaces of an object to calculate the volume of said object. This is achieved by summation of
        signed volumes of corresponding tetrahedra of object surface triangles and outside point.

        Input:  - cityjson
        Output: - void, adds attribute volume to Building objects in cityjson
     */
    std::vector<double> outside_point = { j["metadata"]["geographicalExtent"][0].get<double>() - 1.,
                                          j["metadata"]["geographicalExtent"][1].get<double>() - 1.,
                                          j["metadata"]["geographicalExtent"][2].get<double>() - 1.};

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

                            vertex_cur1 = {X1, Y1, Z1}; vertices_current.emplace_back(vertex_cur1);
                            vertex_cur2 = {X2, Y2, Z2}; vertices_current.emplace_back(vertex_cur2);
                            vertex_cur3 = {X3, Y3, Z3}; vertices_current.emplace_back(vertex_cur3);
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
            co.value()["attributes"]["volume"] = std::to_string(volume);
        }
    }
}

// Number of floors
void getCOBuildingHeights(json &j) {
    /*
        This is the function to calculate the number of floors in the building based on the cityObject attributes for each objects:
        The attributes that are needed: h_dak_min, h_dak_max, m_maaiveld

        Input:  - &j (json)
        Output: - no_floors (int)
     */
    double dak_min, dak_max, dak_dif, maaiveld;
    int no_floor;

    for (auto &co: j["CityObjects"].items()) {

        if (co.value()["type"] == "Building" && co.value()["attributes"]["dak_type"] != "no points"){

            dak_min = co.value()["attributes"]["h_dak_min"];
            dak_max = co.value()["attributes"]["h_dak_max"];
            maaiveld = co.value()["attributes"]["h_maaiveld"];
            dak_dif = ((((dak_max - dak_min) * 0.7) + dak_min) - maaiveld) / 3.0;

            // Check if the difference between the difference in the tolerance of 87%,
            // which corresponds to 2.6m and will count as one floor
            if (dak_dif / ceil(dak_dif) >= 0.87) {no_floor = ceil(dak_dif);}
            else {no_floor = dak_dif;}

            // write value to json file
            co.value()["attributes"]["no_floor"] = no_floor;
        }
    }
}

// Orientation
std::vector<double> surfaceNormal(std::vector<std::vector<double>> &vertices){
    // Surface normal calculation of a given polygon using Newell's method.
    std::vector<double> normal(3);
    double nx = 0., ny = 0., nz = 0.;

    for (int v = 0; v < vertices.size(); v++){
        std::vector<double> current = vertices[v];
        std::vector<double> next = vertices[(v+1)%vertices.size()];
        nx += (current[1] - next[1]) * (current[2] + next[2]);
        ny += (current[2] - next[2]) * (current[0] + next[0]);
        nz += (current[0] - next[0]) * (current[1] + next[1]);
    }
    normal = {nx, ny, nz};
    return normal;
}

std::string interCardinal(std::vector<double> normal) {
    // The orientation is determined by the value of a normal's x and y values, deriving one of the 8 quadrant parts.
    // The 8 mapped quadrant parts relate to the 8 possible intercardinal directions.
    std::string dir;
    double x = normal[0];
    double y = normal[1];

    if (fabs(x) < 2. && fabs(y) < 2.){dir="horizontal";}
    else{
        if (     x < 0. && y > 0. && fabs(y) >= fabs(x)) {dir="NW";}
        else if (x < 0. && y > 0. && fabs(x) >  fabs(y)) {dir="WN";}
        else if (x < 0. && y < 0. && fabs(x) >  fabs(y)) {dir="WS";}
        else if (x < 0. && y < 0. && fabs(y) >= fabs(x)) {dir="SW";}
        else if (x > 0. && y < 0. && fabs(y) >= fabs(x)) {dir="SE";}
        else if (y < 0. && x > 0. && fabs(x) >  fabs(y)) {dir="ES";}
        else if (y > 0. && x > 0. && fabs(x) >  fabs(y)) {dir="EN";}
        else if (y > 0. && x > 0. && fabs(y) >= fabs(x)) {dir="NE";}
    }
    return dir;
}

// Area
std::vector<double> crossProduct(std::vector<double> vect_A, std::vector<double> vect_B) {
    // Calculate cross product of two given vectors.
    std::vector<double> cross_P;
    cross_P.emplace_back(vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1]);
    cross_P.emplace_back(vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2]);
    cross_P.emplace_back(vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0]);
    return cross_P;
}

double areaTriangle(std::vector<double> cross_P) {
    // Area of a triangle by taking half of the area of a parallelogram formed by two given vectors
    double magnitude, area;
    cross_P[0] = (cross_P[0] * cross_P[0]);
    cross_P[1] = (cross_P[1] * cross_P[1]);
    cross_P[2] = (cross_P[2] * cross_P[2]);
    magnitude = sqrt((cross_P[0] + cross_P[1] + cross_P[2]));
    area = magnitude / 2.;
    return area;
}

double distance (std::vector<double> begin_v, std::vector<double> end_v) {
    // Calculate distance between two given vectors (eg to calculate length of an edge)
    std::vector<double> side = {0., 0., 0.};
    std::transform(begin_v.begin(), begin_v.end(), end_v.begin(),
                   side.begin(), std::minus<double>());

    double result = sqrt((side[0]*side[0]) + (side[1] * side[1]) + (side[2] * side[2]));
    return result;
}
