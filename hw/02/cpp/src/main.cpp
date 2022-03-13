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
#include "volume.h"

using json = nlohmann::json;




int   get_no_roof_surfaces(json &j);
void  list_all_vertices(json &j);
void  visit_roofsurfaces(json &j);
void store_all_vertices(json &j, std::vector<std::vector<double>> &all_vertices);



int main(int argc, const char * argv[]) {

  //-- reading the file with nlohmann json: https://github.com/nlohmann/json  
  std::ifstream input("../../data/myfile_new.triangulated.city.json");
  json j;
  input >> j;
  input.close();

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

  //-- add an attribute "volume"
  /*
  for (auto& co : j["CityObjects"]) {
    if (co["type"] == "Building") {
      co["attributes"]["volume"] = rand();
    }
  }
   */


    std::string file_in = "../../data/one_building.obj";

    // ## Read OBJ file ##
    // The vertices and faces are read and stored into vectors.

    std::vector<std::vector<double>> vertices_one_building;
    std::vector<std::vector<int>> face_indices_one_building;
    readObj(file_in, vertices_one_building, face_indices_one_building);

    // ## Create outside point ##
    // with margin, guaranteed to be outside any object.
    // create new vertex outside bbox --> "outside point" for the tetrahedra (stored as an array)
    std::vector<double> outside_point = { j["metadata"]["geographicalExtent"][0].get<double>() - 1.,
                                        j["metadata"]["geographicalExtent"][1].get<double>() - 1.,
                                        j["metadata"]["geographicalExtent"][2].get<double>() - 1.};

    std::vector<int> vi = j["vertices"][0];
    std::vector<double> outside_point_close = {(vi[0] * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>() - 1.,
                                               (vi[1] * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>() - 1.,
                                               (vi[2] * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>() - 1.};


    // ## Calculate volume of object ##
    double volume_sum = volumeObject(outside_point_close, vertices_one_building, face_indices_one_building);

    for (auto& co : j["CityObjects"]) {
        if (co["type"] == "Building") {
            //co["attributes"]["volume"] = volume_sum;
        }
    }



    std::cout << "size of vertices_one_building: " << vertices_one_building.size() << "\n";
    std::cout << "all the vertices_one_building of: " << file_in << "\n";

    /*
    for (int i = 0; i < vertices_one_building.size(); i++) {
        std::cout << "vertex " << i << ":";
        for (auto j : vertices_one_building[i]) {std::cout << " " << j;}
        std::cout << "\n";
    }


       std::cout << "all the face indices of: " << file_in << "\n";

       for (auto i : face_indices_one_building) {
         for (auto j: i) { std::cout << " " << j; }
         std::cout << "\n";
     }
   */

    std::cout << "face_indices_one_building.size(): " << face_indices_one_building.size() << std::endl;


    std::cout << std::setprecision(8) << std::fixed << "volume sum: " << volume_sum << "\n";

    for (auto& co : j["CityObjects"]) {
        int count_children = 0;
        if (co["type"] == "Building") {
            for (auto& i : co["children"]) {
                count_children++;
            }
        }
        if (count_children > 1) {
            //std::cout << "number of children in building " << co << ": " << count_children << "\n";
            //std::cout << co.operator[]("key") << "\n";

        }
    }

    std::vector<std::vector<double>> vertices_parent;
    std::vector<std::vector<int>> face_indices_parent;


    for (auto& co : j["CityObjects"].items()) {

        int count_children2 = 0;
        if (co.value()["type"] == "Building") {
            double volume = 0.;

            for (auto& i : co.value()["children"]) {
                count_children2++;
                //std::cout << "child: " << i.get<std::string>() << "\n";

                for (auto& g : j["CityObjects"][i.get<std::string>()]["geometry"]) {
                    int count_loop = 0;
                    for (int i = 0; i < g["boundaries"].size(); i++) {
                        //std::cout << "g[\"boundaries\"][i].size(): " << g["boundaries"][i].size() << std::endl;
                        for (int k = 0; k < g["boundaries"][i].size(); k++) {
                            std::vector<std::vector<double>> vertices_current;
                            count_loop++;
                            std::vector<double> vertex_cur1; std::vector<double> vertex_cur2; std::vector<double> vertex_cur3;
                            //vertex_cur1 = {(j["vertices"][g["boundaries"][i][k][0][0].get<int>()][0].get<int>() * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>(),
                            //              (j["vertices"][g["boundaries"][i][k][0].get<int>()][1].get<int>() * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>(),
                            //              (j["vertices"][g["boundaries"][i][k][0].get<int>()][2].get<int>() * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>()};
                            //vertex_cur.emplace_back(outside_point);
                            //std::cout << "i: " << i << "k: " << k << std::endl;
                            //std::cout << g["boundaries"][i][k] << std::endl;
                            //std::cout << " g[\"boundaries\"][i][j][0][0]: " << g["boundaries"][i][k][0][0] << std::endl;
                            //std::cout << " j[\"vertices\"][g[\"boundaries\"][i][k][0][0].get<int>()]: " << j["vertices"][g["boundaries"][i][k][0][0].get<int>()]<< std::endl;
                            //std::cout << " g[\"boundaries\"][i][j][0][0]: " << g["boundaries"][i][k][0][0] << std::endl;
                            //std::cout << " g[\"boundaries\"][i][j][0][0]: " << g["boundaries"][i][k][0][0] << std::endl;
                            //std::cout << " g[\"boundaries\"][i][j] X: "<< j["vertices"][g["boundaries"][i][k][0][0].get<int>()][0] << std::endl;
                            //std::cout << " g[\"boundaries\"][i][j] X: "<< j["vertices"][g["boundaries"][i][k][0][0].get<int>()][0].get<int>() << std::endl;
                            //std::cout << " g[\"boundaries\"][i][j] Y: "<< j["vertices"][g["boundaries"][i][k][0][0].get<int>()][1].get<int>() << std::endl;
                            //std::cout << " g[\"boundaries\"][i][j] Z: "<< j["vertices"][g["boundaries"][i][k][0][0].get<int>()][2].get<int>() << std::endl;

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

                            /*
                            for (auto i : vertex_cur1) {
                                std::cout << i << " ";
                            }
                            std::cout << std::endl;

                            for (auto i : vertex_cur2) {
                                std::cout << i << " ";
                            }
                            std::cout << std::endl;

                            for (auto i : vertex_cur3) {
                                std::cout << i << " ";
                            }
                            std::cout << std::endl;
                             */

                            double mat_tetra_current[N][N];
                            fillMatrix4x4(mat_tetra_current, vertices_current, N);
                            double determinant_tetra_current = determinantOfMatrix(mat_tetra_current, N);
                            volume += volumeTetra(determinant_tetra_current);

                            //double x = j["vertices"][g["boundaries"][i][j][0][0]][0];
                            //std::cout << "Surface: " << j["vertices"][g["boundaries"][i][j][0][0].get<int>()] << std::endl;
                            //std::cout << "first vertex: " << g["boundaries"][i][k][0][0].get<int>() << "\n";
                            //std::cout << "hallooo: " << (j["vertices"][g["boundaries"][i][k][0][0].get<int>()][0].get<int>() * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>() << ", "
                            //<< (j["vertices"][g["boundaries"][i][k][0][0].get<int>()][1].get<int>() * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>() << ", "
                            //<< (j["vertices"][g["boundaries"][i][k][0][0].get<int>()][2].get<int>() * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>()<< "\n";
                        }
                    }
                    //std::cout << "CHECK COUNT_LOOP: " << count_loop << std::endl;
                }
            }

            //std::cout << "VOLUME BRO: " << volume << std::endl;
            std::string unit = "m^3";
            co.value()["attributes"]["volume"] = std::to_string(volume) + unit;
            //std::cout << co.key() << "\n";
        }
        //std::cout << co.key() << "\n";
        //std::cout << "count children 2: " << count_children2 << std::endl;

    }

    std::vector<std::vector<double>> all_vertices;
    store_all_vertices(j, all_vertices);

    std::cout << "all_vertices.size(): " << all_vertices.size() << "\n";


    double x = vi[0] * j["transform"]["scale"][0].get<double>();
    std::cout << "first vertex: " << j["vertices"][0][0] << "\n";
    std::cout << "scale thing: " << j["transform"]["scale"][0].get<double>();


    //-- write to disk the modified city model (myfile.city.json)
    std::ofstream o("../../data/myfile_new_with_volumes.triangulated.city.json");
    o << j.dump(2) << std::endl;
    o.close();

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