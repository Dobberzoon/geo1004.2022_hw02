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

int main(int argc, const char * argv[]) {

    std::cout << "reading data";
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
    std::cout << "---> data is read\n";

    // Get volume of all CityObjects, and update the cityjson
    std::cout << "calculating CityObjects' volumes";
    getCOVolumes(j2);
    std::cout << "---> done\n";

    // Get number of floors of all CityObjects, and update the cityjson
    std::cout << "calculating CityObjects' number of floors";
    getCONoFloors(j2);
    std::cout << "---> done\n";

    // Get the area and orientation of CityObject's roofSurface
    std::cout << "calculating CityObjects' roofSurfaces' area + orientation";
    getCOAreaOrientation(j);
    std::cout << "---> done\n";

    std::cout << "writing to disk";
    //-- write to disk the modified city model (myfile.city.json)
    std::string file_out1 = "../../data/3dbag_original_upgraded+area+orientation_TEST3.json";
    std::ofstream o(file_out1);
    o << j.dump(2) << std::endl;
    o.close();

    //-- write to disk the modified city model (myfile.city.json)
    std::string file_out2 = "../../data/3dbag_triangulated+volume+floor_TEST3.json";
    std::ofstream o2(file_out2);
    o2 << j2.dump(2) << std::endl;
    o2.close();
    std::cout << "---> done\n";
    std::cout << "written to:\n" << file_out1 << "\n" << file_out2 << "\n";

    return 0;
}



