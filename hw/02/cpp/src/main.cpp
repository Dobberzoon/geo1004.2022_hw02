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
#include <cmath>
#include "json.hpp"

using json = nlohmann::json;


//double ax,ay,bx,by;
std::vector<std::vector<double>>vic;

int   get_no_roof_surfaces(json &j);
void  list_all_vertices(json& j);
void  visit_roofsurfaces(json &j);
void no_of_floors (json &j);
void vertices_roof(json &j);
void  list_all_vertices01(json& j);
void roofSurfaceArea (json& j);
void extractPoly(json& j);
//void orientation(north,norm);
//std::vector<std::vector<double>>test={{-2,-5},{1,3},{-1,0}};
std::vector<float>north={0,1,0};
//std::vector<float>norm;

float distance(double ax,double ay,double bx,double by)
{
float dist=sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
return dist;
}

float heron(std::vector<std::vector<double>>vic){
    float sp;
    float a, b, c;
    float area;
    a=distance(vic[0][0],vic[0][1],vic[1][0],vic[1][1]);
    b=distance(vic[1][0],vic[1][1],vic[2][0],vic[2][1]);
    c=distance(vic[2][0],vic[2][1],vic[0][0],vic[0][1]);

    sp=(a+b+c)/2;

    area=sqrt(sp*(sp-a)*(sp-b)*(sp-c));
    return area;
}
/*double ax0, ay0, bx0, by0, cx0, cy0;
float heron(ax0,ay0,bx0,by0,cx0,cy0)
{
    float sp;
    float a, b, c;
    float area;
    a=distance(ax0,ay0,bx0,by0);
    b=distance(bx0,by0,cx0,cy0);
    c=distance(cx0,cy0,ax0,ay0);

    sp=(a+b+c)/2;

    area=sqrt(sp*(sp-a)*(sp-b)*(sp-c));
    return area;
}*/
std::string orientation(std::vector<double>norm){
    std::string dir;
    double x=norm[0];
    double y=norm[1];
    double z=norm[2];


     if(z<0.1 && z>-0.1){dir="horizontal";}
     else{
     if(x==0 && y>0) {dir="N";}
     else if(x<0 && y>0) {dir="NW";}
     else if(x<0 && y==0) {dir="W";}
     else if(x<0 && y<0) {dir="SW";}
     else if(x==0 && y<0) {dir="S";}
     else if(y<0 && x>0) {dir="SE";}
     else if (y==0 && x>0) {dir="E";}
     else if (y>0 && x>0) {dir="NE";}
     }
     return dir;}

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
    //std::cout << "normal vector: " << nx<< ", " << ny << ", " << nz << std::endl;
    return normal;
}

int main(int argc, const char * argv[]) {
  //-- reading the file with nlohmann json: https://github.com/nlohmann/json  
  std::ifstream input("../../data/myfile.city.json");
  json j;
  input >> j;
  input.close();

  //-- get total number of RoofSurface in the file
  // int noroofsurfaces = get_no_roof_surfaces(j);
  // std::cout << "Total RoofSurface: " << noroofsurfaces << std::endl;

   //list_all_vertices(j);

  //visit_roofsurfaces(j);

  //no_of_floors(j);
    //vertices_roof;
    //list_all_vertices01(j);
    //roofSurfaceArea(j);
    extractPoly(j);


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

  /*for (auto& co : j["CityObjects"]) {
      if (co["type"] == "Building") {
            co["attributes"]["no. of floors"] = no_of_floors(co);
      }
  }*/

  //-- write to disk the modified city model (myfile.city.json)
  std::ofstream o("../../data/yone.json");
  o << j.dump(2) << std::endl;
  o.close();

  //float x=heron(test);
  //std::cout<<"simple heron "<<x;
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

void no_of_floors(json& j) {
float hmax;
float hmin;
float hgrlvl;
float rdif;
    for (auto &co: j["CityObjects"]) {

        if (co["type"] == "Building") {
            //std::cout << "= CityObject: " << co.key() << std::endl;
            hmax=co["attributes"]["h_dak_max"];
            hmin=co["attributes"]["h_dak_min"];
            hgrlvl=co["attributes"]["h_maaiveld"];
            rdif=((hmax-hmin)*0.7+(hmin-hgrlvl))/3.0;
            std::cout<<round(rdif)<<"\n";

            }
        }
    }


/*float distance(double ax,double ay,double bx,double by);
{
  float dist=sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
  return dist;
}*/

void vertices_roof(json& j) {
    /*for (auto &co: j["CityObjects"].items()) {
        for (auto &g: co.value()["geometry"]) {
            for (auto &shell0: g["semantics"]["values"]){
                for (auto &s: shell0){
                    if (g["semantics"]["surfaces"][s.get<int>()]["type"].get<std::string>().compare("RoofSurface") == 0){
                        for(auto &shell1: g["boundaries"])

                    }
                }
            }}
        }*/
        }

/*void list_all_vertices01(json& j) {

    float ar, roof_area = 0.00;
    std::vector<std::vector<double>> triangle;
    for (auto &co: j["CityObjects"].items()) {
        roof_area = 0.00;
        std::cout << "= CityObject: " << co.key() << std::endl;
        for (auto &g: co.value()["geometry"]) {
            if (g["type"] == "Solid") {
                for (auto &shell0: g["semantics"]["values"]) {
                    for (auto &s: shell0) {
                        if (g["semantics"]["surfaces"][s.get<int>()]["type"].get<std::string>().compare(
                                "RoofSurface") == 0) {
                            for (auto &shell: g["boundaries"]) {
                                for (auto &surface: shell) {
                                    //std::vector<std::vector<double>> triangle;
                                    for (auto &ring: surface) {
                                        //std::cout << "---" << std::endl;
                                        for (auto &v: ring) {
                                            std::vector<double> coord;
                                            std::vector<int> vi = j["vertices"][v.get<int>()];
                                            double x = (vi[0] * j["transform"]["scale"][0].get<double>()) +
                                                       j["transform"]["translate"][0].get<double>();
                                            double y = (vi[1] * j["transform"]["scale"][1].get<double>()) +
                                                       j["transform"]["translate"][1].get<double>();
                                            //double z = (vi[2] * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>();
                                            //std::cout<< v << " (" << x << ", " << y << ", " << z << ")" << std::endl;
                                            coord.emplace_back(x);
                                            coord.emplace_back(y);
                                            triangle.emplace_back(coord);
                                        }

                                        ar = heron(triangle);
                                        roof_area = ar + roof_area;
                                    }
                                    //std::cout<<roof_area<<std::endl;

                                }
                            }
                        }
                    }
                }
                //std::cout<<"Area of roof "<<roof_area<<std::endl;

                //std::cout << "Area of roof " << roof_area << std::endl;
            }

            std::cout<<"Area of roof "<<roof_area<<std::endl;}
    }

}
*/

/*for(auto i:triangle)
{std::cout<<i[0]<<" "<<i[1]<<std::endl;}*/
/*float heron(std::vector<std::vector<double>>vic1){
    float sp;
    float a, b, c;
    float area;
    a=distance(vic1[0][0],vic1[0][1],vic1[1][0],vic1[1][1]);
    b=distance(vic1[1][0],vic1[1][1],vic1[2][0],vic1[2][1]);
    c=distance(vic1[2][0],vic1[2][1],vic1[0][0],vic1[0][1]);

    sp=(a+b+c)/2;

    area=sqrt(sp*(sp-a)*(sp-b)*(sp-c));
    return area;
}*/

void roofSurfaceArea(json &j) {
    for (auto &co: j["CityObjects"].items()) {
        for (auto &g: co.value()["geometry"]) {
            if (g["type"] == "Solid") {
                double area = 0;
                for (int i = 0; i < g["boundaries"].size(); i++) {
                    for (int k = 0; k < g["boundaries"][i].size(); k++) {
                        int sem_index = g["semantics"]["values"][i][k];
                        if (g["semantics"]["surfaces"][sem_index]["type"].get<std::string>().compare("RoofSurface") ==
                            0)
                        {
                            std::cout << "RoofSurface: " << g["boundaries"][i][k] << std::endl;
                            std::vector<std::vector<double>> triangle;
                            std::vector<double> a;
                            std::vector<double> b;
                            std::vector<double> c;
                            int ax, ay, bx, by, cx, cy;
                            double AX, AY, BX, BY, CX, CY;
                            ax = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][0].get<int>();
                            ay = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][1].get<int>();

                            bx = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][0].get<int>();
                            by = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][1].get<int>();

                            cx = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][0].get<int>();
                            cy = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][1].get<int>();

                            AX = (ax * j["transform"]["scale"][0].get<double>()) +
                                 j["transform"]["translate"][0].get<double>();
                            AY = (ay * j["transform"]["scale"][1].get<double>()) +
                                 j["transform"]["translate"][1].get<double>();

                            BX = (bx * j["transform"]["scale"][0].get<double>()) +
                                 j["transform"]["translate"][0].get<double>();
                            BY = (by * j["transform"]["scale"][1].get<double>()) +
                                 j["transform"]["translate"][1].get<double>();

                            CX = (cx * j["transform"]["scale"][0].get<double>()) +
                                 j["transform"]["translate"][0].get<double>();
                            CY = (cy * j["transform"]["scale"][1].get<double>()) +
                                 j["transform"]["translate"][1].get<double>();

                            a = {AX, AY};
                            triangle.emplace_back(a);
                            b = {BX, BY};
                            triangle.emplace_back(b);
                            c = {CX, CY};
                            triangle.emplace_back(c);
                            area = heron(triangle)+area;
                            std::cout << "area triangle: " << area << std::endl;}
                            }
                        }
                std::cout << "Area of NEW ROOF BRO: " << area << std::endl;
                std::cout << co.key() << std::endl;
                    }
                }

            }
        }

void extractPoly(json &j) {
    for (auto &co: j["CityObjects"].items()) {
        for (auto &g: co.value()["geometry"]) {
            if (g["type"] == "Solid") {
                std::cout<<"Orientation of surfaces of house"<<"\n";
                int sem_index_extended=0;
                for (int i = 0; i < g["boundaries"].size(); i++) {
                    for (int k = 0; k < g["boundaries"][i].size(); k++) {
                        int sem_index = g["semantics"]["values"][i][k];
                        sem_index_extended = g["semantics"]["surfaces"].size();
                        //std::cout<<"Orientation of surfaces of house"<<"\n";
                        if (g["semantics"]["surfaces"][sem_index]["type"].get<std::string>().compare("RoofSurface") ==
                            0) {
                            double orient;
                            std::vector<std::vector<double>> coords;
                            int ax, ay, az, bx, by, bz, cx, cy, cz;
                            double p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z;

                            ax = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][0].get<int>();
                            ay = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][1].get<int>();
                            az = j["vertices"][g["boundaries"][i][k][0][0].get<int>()][2].get<int>();

                            bx = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][0].get<int>();
                            by = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][1].get<int>();
                            bz = j["vertices"][g["boundaries"][i][k][0][1].get<int>()][2].get<int>();

                            cx = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][0].get<int>();
                            cy = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][1].get<int>();
                            cz = j["vertices"][g["boundaries"][i][k][0][2].get<int>()][2].get<int>();

                            p1x = (ax * j["transform"]["scale"][0].get<double>()) +
                                  j["transform"]["translate"][0].get<double>();
                            p1y = (ay * j["transform"]["scale"][1].get<double>()) +
                                  j["transform"]["translate"][1].get<double>();
                            p1z = (az * j["transform"]["scale"][2].get<double>()) +
                                  j["transform"]["translate"][2].get<double>();

                            p2x = (bx * j["transform"]["scale"][0].get<double>()) +
                                  j["transform"]["translate"][0].get<double>();
                            p2y = (by * j["transform"]["scale"][1].get<double>()) +
                                  j["transform"]["translate"][1].get<double>();
                            p2z = (bz * j["transform"]["scale"][2].get<double>()) +
                                  j["transform"]["translate"][2].get<double>();

                            p3x = (cx * j["transform"]["scale"][0].get<double>()) +
                                  j["transform"]["translate"][0].get<double>();
                            p3y = (cy * j["transform"]["scale"][1].get<double>()) +
                                  j["transform"]["translate"][1].get<double>();
                            p3z = (cz * j["transform"]["scale"][2].get<double>()) +
                                  j["transform"]["translate"][2].get<double>();

                            std::vector<double> P1 = {p1x, p1y, p1z};
                            std::vector<double> P2 = {p2x, p2y, p2z};
                            std::vector<double> P3 = {p3x, p3y, p3z};
                            coords.emplace_back(P1);
                            coords.emplace_back(P2);
                            coords.emplace_back(P3);
                            /*for (auto i:coords){
                                for(auto j:i){
                                std::cout<<j<<" ";
                                std::cout<<"\n";}}*/
                            std::vector<double>normie= surfaceNormal(coords);

                            /*for(int i=0; i<normie.size();i++){
                                std::cout<<normie[i]<<' '<<"\n";
                            }*/
                            std::cout<<"\n";
                            std::cout<<orientation(normie);
                            //std::cout<<orient<<"\n";
                            g["semantics"]["surfaces"][sem_index_extended]["type"] = "RoofSurface";
                            g["semantics"]["surfaces"][sem_index_extended]["orientation"] = orientation(normie);
                            g["semantics"]["values"][i][k] = sem_index_extended;
                            sem_index_extended++;
                        }
                    }


                }
            }
        }
    }
}
