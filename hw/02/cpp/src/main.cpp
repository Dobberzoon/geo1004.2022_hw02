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
#include <vector>
#include <string>
#include "json.hpp"
#include "volume.h"

using json = nlohmann::json;



int   get_no_roof_surfaces(json &j);
void  list_all_vertices(json& j);
void  visit_roofsurfaces(json &j);
std::vector<double> surfaceNormal(std::vector<std::vector<double>> &vertices);




int main(int argc, const char * argv[]) {

  //-- reading the file with nlohmann json: https://github.com/nlohmann/json  
  std::ifstream input("../../data/triangulated_sample_building.city.json");
  json j;
  input >> j;
  input.close();


  // create new vertex outside bbox --> "outside point" for the tetrahedra (stored as an array)
  double op[] = {j["metadata"]["geographicalExtent"][0].get<double>() - 1,
          j["metadata"]["geographicalExtent"][1].get<double>() - 1,
          j["metadata"]["geographicalExtent"][2].get<double>() - 1};

  // print the outside point
  std::cout << "op: ";
    for(int i=0;i<3;i++)
        std::cout<<op[i]<<" ";
    std::cout << "\n";


  //-- get total number of RoofSurface in the file
   int noroofsurfaces = get_no_roof_surfaces(j);
   std::cout << "Total RoofSurface: " << noroofsurfaces << std::endl;

//  list_all_vertices(j);

  visit_roofsurfaces(j);

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

  //-- add an attribute "volume"
  for (auto& co : j["CityObjects"]) {
    if (co["type"] == "Building") {
      co["attributes"]["volume"] = rand();
    }
  }

  //-- write to disk the modified city model (myfile.city.json)
  std::ofstream o("myfile.city.json");
  o << j.dump(2) << std::endl;
  o.close();

  std::string file_in = "../../data/big_tetrahedron.obj";

  // ## Read OBJ file ##
  // The vertices and faces are read and stored into vectors.

  std::vector<std::vector<double>> vertices;
  std::vector<std::vector<int>> face_indices;

  readObj(file_in, vertices, face_indices);

  std::cout << "size of vertices: " << vertices.size() << "\n";
  std::cout << "all the vertices of: " << file_in << "\n";

  for (int i = 0; i < vertices.size(); i++) {
      std::cout << "vertex " << i << ":";
      for (auto j : vertices[i]) {std::cout << " " << j;}
      std::cout << "\n";
  }

    std::cout << "all the face indices of: " << file_in << "\n";

  for (auto i : face_indices) {
      for (auto j: i) { std::cout << " " << j; }
      std::cout << "\n";
  }

  double mat_import[N][N];

  // filling matrix from obj vertex list (will be similar to vertex list from json)
  initMatrix4x4(mat_import, vertices, N);

  std::cout << "all the contents of mat_import:\n";

  for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
          std::cout << mat_import[i][j] << " ";
      }
      std::cout << "\n";
  }


//  int mat[N][N] = {{2, 1, 3}, {6, 5, 7}, {4, 9, 8}};

// For this the determinant will be positive
/*    int mat[N][N] = {{ 1,  1,  1,  1},
                       {-1,  1, -1,  1},
                       {-1, -1,  1,  1},
                       { 1, -1, -1,  1}};*/

// For this the determinant will be negative, and double precision

  double mat[N][N] = {{ 1.01,  1.01,  1.01,  1.01},
                   {-1.01, -1.01,  1.01,  1.01},
                   {-1.01,  1.01, -1.01,  1.01},
                   { 1.01, -1.01, -1.01,  1.01}};
  std::cout << "Determinant: " << determinantOfMatrix(mat, N) << std::endl;
    std::cout << "Determinant of " << file_in << ": " << determinantOfMatrix(mat_import, N) << std::endl;

  // list_all_vertices(j);

  // initialise variable for volume
  double vol = 0;

  // get all buildingParts per building
  for (auto& co: j["CityObjects"].items()){
      //print key value pair
      std::cout <<"key: " << co.key() << "value: " << co.value() << std::endl;

      // get volume of building

      for (auto& g : co.value()["children"].items()){
          std::cout << "children: " << g.value() << std::endl;

      }

  }


  std::vector<std::vector<double>> test_vector;
  std::vector<double> p0 = {6,1,4};
  std::vector<double> p1 = {7,0,9};
  std::vector<double> p2 = {1,1,2};
  test_vector = {p0, p1, p2};

  std::vector<double> s_normal = surfaceNormal(test_vector);


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
                //std::cout << std::setprecision(2) << std::fixed << v << " (" << vi[0] << ", " << vi[1] << ", " << vi[2] << ")" << std::endl;
              }
            }
          }
        }
      }
    }
  }
}
// get polygon's normal with Newell's method
std::vector<double> surfaceNormal(std::vector<std::vector<double>> &vertices){
    // empty vector and empty variables to hold the normal later on
    std::vector<double> normal(3);
    double nx = 0, ny = 0, nz = 0;

    for (int v = 0; v < vertices.size(); v++){
        std::vector<double> current = vertices[v];
        std::vector<double> next = vertices[(v+1)%vertices.size()];

        nx += (current[1] - next[1]) * (current[2] + next[2]);
        ny += (current[2] - next[2]) * (current[0] + next[0]);
        nz += (current[0] - next[0]) * (current[1] + next[1]);
    }

    //double norm = std::sqrt( pow(nx, 2) + pow(ny, 2) + pow(nz, 2));
    normal = {nx, ny, nz};
    std::cout << "normal vector: " << nx<< ", " << ny << ", " << nz << std::endl;
    return normal;
    }