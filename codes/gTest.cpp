#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Tet/IOtet.h"
#include "Tet/Mesh3D.h"
#include "Tet/Point3D.cpp"
#include "Tet/Definition.h"
/*std::vector<int> removeDupWord(std::string str, std::string vari)
{
    // Used to split string around spaces.
    std::vector<int> ids;
    std::istringstream ss(str);

    std::string word; // for storing each word

    // Traverse through all words
    // while loop till we get
    // strings to store in string word
    while (ss >> word)
    {
        if(word == vari) {
            continue;
        }
        std::stringstream cvt(word);
        int id=0;
        cvt >> id;
        ids.push_back(id);
        // print the read word
        std::cout << word << "\n";
    }
    return ids;
}*/


int main(int argc, char **argv) {
    TMeshptr mesh=ReadVtk("tetalone .vtk");
 // TMeshptr mesh = ReadVtk("/home/aravindhkumar.kalimuthu/Codes/Merge/cmake-build-debug/code/mergeLib/out.vtk");
 // mesh->writeTet("tetalone");
  /*  Point_3 * obj1 = new Point_3 (1.1,2.3,3);
    Point_3 * obj2 = new Point_3 (1.1,2,3);

    Point_3 obj(1.1,2.3,3);
    Point_3 obj_(1.1,2,3);

    std::map<Point_3,int> normaltoint;
    std::map<Point_3*,int,ComparePoint3D<double,3>> normalptrtoint;

    normaltoint.insert({obj,1});
    normaltoint.insert({obj_,2});

    normalptrtoint.insert({obj1,1});
    normalptrtoint.insert({obj2,2});

    for(auto ele :normaltoint )
    {
        std::cout << ele.first << " " << ele.second << std::endl;
    }
    std::cout << " ------------- xxx ---------------- " << std::endl;
    for(auto ele :normalptrtoint )
    {
        std::cout << *ele.first << " " << ele.second << std::endl;
    }

    if(*obj1<*obj2) {
        std::cout << "yes" << std::endl;
    } else {
        std::cout << "noooo" << std::endl;
    }*/
  /*  std::cout << obj[2] << std::endl;
    obj[0]=5;
    std::cout << obj[0] << " "   <<obj[1] << " " <<   obj[2]<< std::endl;
*/

}
 