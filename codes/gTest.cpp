#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Tet/IOtet.h"
#include "Tet/Mesh3D.h"

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

void fun( )
{
    int arr[7]={15,-1,-4,20,25,545,5};
    for(int i=0; i<7; i++)
    {
        for(int j=i+1; j<7; j++)
        {
            if(arr[i] > arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    for(int i=0; i<7; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}






int main(int argc, char **argv) {
    TMeshptr tmesh = nullptr;

  //   const std::string filename ="/home/aravindhkumar.kalimuthu/Desktop/practice/ProjectSKA/cmake-build-debug/codes/shared.vtk";
    //    const std::string filename ="/home/aravindhkumar.kalimuthu/Desktop/practice/ProjectSKA/cmake-build-debug/codes/ch.vtk";
//    Point_3 *a = new Point_3 (1.0,2.0,3.0);
//    Point_3 *b = new  Point_3 (4.0,5.0,6.0);
//    a->setID(10);
//    b->setID(12);
//
//    Point_3 *c = new Point_3 (1.0,2.0,3.0);
//    Point_3 *d = new  Point_3 (4.0,5.0,6.0);
//    EdgePts ed1(b,a);
//    std::cout << ed1.hash() << std::endl;
//    EdgePts ed2(d,c);
//    if(ed1 == ed2)
//    {
//        std::cout << "yes it is true " << std::endl;
//    }


    const std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/ProjectSKA/codes/tetalone.vtk";
    // TMeshptr mesh = ReadVtk("/home/aravindhkumar.kalimuthu/Codes/Merge/cmake-build-debug/code/mergeLib/out.vtk");
    // mesh->writeTet("tetalone");

    try {
        tmesh = ReadVtk(filename);
    }
    catch (const std::string &msg) {
        std::cout << msg << std::endl;
    }
    tmesh->getborder();
//

}

 //tmesh->writeTet("tetalone");
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


 