#include "newattributes.h"

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
                while (iss >> word) face.emplace_back(std::stoi(word) - 1);
                face_indices.push_back(face);
            }
        }
    }
    stream_in.close();
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
        and writes it as the attribute "volume" to each CityObject in m^3

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
            co.value()["attributes"]["volume"] = std::to_string(volume);
        }
    }
}

void getCOBuildingHeights(json &j) {
    float dak_min, dak_max, dak_dif, maaiveld, h_from_ground ;
    int no_floor;

    for (auto &co: j["CityObjects"].items()) {
//        std::cout << "CityObject: " << co.key() << std::endl;
        if (co.value()["type"] == "Building" && co.value()["attributes"]["dak_type"] == "no points"){
            std::cout << co.key() << std::endl;
            /*

             std::cout << co.value()["attributes"]["dak_type"] << "\n";
            dak_min = co.value()["attributes"]["h_dak_min"];
            dak_max = co.value()["attributes"]["h_dak_max"];
            maaiveld = co.value()["attributes"]["h_maaiveld"];
            h_from_ground = ((((dak_max - dak_min) * 0.7) + dak_min) - maaiveld);
            dak_dif = ((((dak_max - dak_min) * 0.7) + dak_min) - maaiveld) / 3.0;
            */
            //std::cout << "Dak Max: " << dak_max << std::endl;
            //std::cout << "Dak Min: " << dak_min << std::endl;
            //std::cout << "Maaiveld: " << maaiveld << std::endl;
            //std::cout << "Height from Ground: " << h_from_ground << std::endl;

            /*
            if (dak_dif / ceil(dak_dif) >= 0.87) {no_floor = ceil(dak_dif);}
            else {no_floor = dak_dif;}

            std::cout << "No of Floors: " << no_floor << std::endl;
             */
            //std::cout << std::endl;
            //co.value()["attributes"]["no_floor"] = no_floor;
        }

    }

}

// OLD CODE, TO BE REMOVED FOR FINAL VERSION:

/*
double volumeObject(std::vector<double> &outside_point, std::vector<std::vector<double>> &vertices_object,
                    std::vector<std::vector<int>> &face_indices_object) { */
    /*
        volumeObject takes an outside point and triangulated surfaces of an object to calculate the volume
        of said object. This is achieved by summation of signed volumes of corresponding tetrahedra of object
        surface triangles and outside point.

        Input:  - Outside point (eg extreme of data extent +/- a margin), in vorm of a coordinate vector.
                - vertex list of (triangulated) object
                - surfaces list of (triangulated) object
        Output: - volume (in given units, double precision)
     */


/*
double result = 0.;
for (auto i : face_indices_object) {
std::vector<std::vector<double>> vertices_current;
for (auto j : i) {
std::vector<double> vertex;
for (auto k : vertices_object[j]) {
vertex.emplace_back(k);
}
vertices_current.emplace_back(vertex);
}
vertices_current.emplace_back(outside_point);
double mat_tetra_current[N][N];
fillMatrix4x4(mat_tetra_current, vertices_current, N);
double determinant_tetra_current = determinantOfMatrix(mat_tetra_current, N);
result += volumeTetra(determinant_tetra_current);
}
return result;
}
 */

