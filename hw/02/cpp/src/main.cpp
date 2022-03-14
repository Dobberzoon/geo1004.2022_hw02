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
     _       _      _
   __| | __ _| | __ | |__   _____   _____ _ __
  / _` |/ _` | |/ / | '_ \ / _ \ \ / / _ \ '_ \
 | (_| | (_| |   <  | |_) | (_) \ V /  __/ | | |
  \__,_|\__,_|_|\_\ |_.__/ \___/ \_/ \___|_|_|_|   _
  _ __ ___ (_)(_)_ __   | |__   ___   ___  / _| __| |
 | '_ ` _ \| || | '_ \  | '_ \ / _ \ / _ \| |_ / _` |
 | | | | | | || | | | | | | | | (_) | (_) |  _| (_| |
 |_| |_| |_|_|/ |_| |_| |_| |_|\___/ \___/|_|  \__,_|
            |__/

*/

#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

int     get_no_roof_surfaces(json &j);
void    list_all_vertices(json& j);
void    visit_roofsurfaces(json &j);
void    get_roof_height(json&j);
void    get_building_height(json &j);

int main(int argc, const char * argv[]) {

    //-- reading the file with nlohmann json: https://github.com/nlohmann/json
    std::ifstream input("../../data/twobuildings.city.json");
    json j;
    input >> j;
    input.close();

    //-- get total number of RoofSurface in the file
    // int noroofsurfaces = get_no_roof_surfaces(j);
    // std::cout << "Total RoofSurface: " << noroofsurfaces << std::endl;

    // list_all_vertices(j);

//    visit_roofsurfaces(j);
    get_building_height(j);

//    for (auto& element : j["CityObjects"]) {
//        std::cout << element << std::endl;
//    }


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

    return 0;
}


// Visit every 'RoofSurface' in the CityJSON model and output its geometry (the arrays of indices)
// Useful to learn to visit the geometry boundaries and at the same time check their semantics.

// interviene from one specifi solids and accumulates all the visited surfaces
// now you need to  interviene from one specifi solids and accumulates all the visited surfaces
// now you need to programme into a vector and make sure

void visit_roofsurfaces(json &j) {
    for (auto& co : j["CityObjects"].items()) {
//        std::vector<double> roof_surface; // put the roof surfaces and herons formula and list all vertices
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
//            heronsArea(roof_surface);
        }
    }
}

//void heronsArea(&roof_surface){
//    for (auto roof : roof_surface);
//
//}




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

void get_roof_height(json &j) {
    float dak_min;
    float dak_max;
    float maaiveld;
    int no_floors;
//    float b_height = 0;
    for (auto& [key, value]: j["CityObjects"].items()) {
        for (auto& co : j["CityObjects"]) {
            if (co["type"] == "Building") {
                std::cout << key << "...." << std::endl;
                dak_min = co["attributes"]["h_dak_min"];
                dak_max = co["attributes"]["h_dak_max"];
                maaiveld = co["attributes"]["h_maaiveld"];
                no_floors = ceil((0.7 * (dak_max - dak_min)) - maaiveld);
                std::cout << no_floors << std::endl;
            }
        }
    }
}

void get_building_height(json &j) {
    float dak_min, dak_max, dak_dif, maaiveld, h_from_ground;
    int no_floors;
    for (auto &co: j["CityObjects"].items()) {
//        std::cout << "CityObject: " << co.key() << std::endl;
        if (co.value()["type"] == "Building"){
            std::cout << co.key() << std::endl;
            dak_min = co.value()["attributes"]["h_dak_min"];
            dak_max = co.value()["attributes"]["h_dak_max"];
            maaiveld = co.value()["attributes"]["h_maaiveld"];
            h_from_ground = ((((dak_max - dak_min) * 0.7) + dak_min) - maaiveld);
            dak_dif = ((((dak_max - dak_min) * 0.7) + dak_min) - maaiveld) / 3.0;

            std::cout << "Dak Max: " << dak_max << std::endl;
            std::cout << "Dak Min: " << dak_min << std::endl;
            std::cout << "Maaiveld: " << maaiveld << std::endl;
            no_floors = dak_dif;

            std::cout << "Height from Ground: " << h_from_ground << std::endl;
            std::cout << "No of Floors: " << no_floors << std::endl;
            std::cout << std::endl;
        }
    }
}

//for (auto& co : j["CityObjects"].items()) {
//std::cout << "= CityObject: " << co.key() << std::endl;
//for (auto& g : co.value()["geometry"]) {






////        for (auto &co: j["CityObjects"]) {
//            if (co["type"] == "Building") {
////            std::cout << "= CityObject: " << co.key() << std::endl;
//                dak_min = co["attributes"]["h_dak_min"];
//                dak_max = co["attributes"]["h_dak_max"];
//                maaiveld = co["attributes"]["h_maaiveld"];
//                dak_dif = ((0.7 * (dak_max - dak_min)) - maaiveld) / 3.0;
//                dak_dif = ceil(dak_dif);
//                std::cout << "building height: " << dak_dif << std::endl;
////            }
//        }
//    }


