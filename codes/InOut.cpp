#include "InOut.h"


std::shared_ptr<Mesh> ReadSTL(std::string filename){
    stl_reader::StlMesh<float, unsigned int> mesh(filename);
    size_t no_of_solids = mesh.num_solids();
    std::cout<<"Number of solids in given file "<<no_of_solids<<". "<<" "<<"Always Returns First Solid!! "<<std::endl;
    std::shared_ptr<Mesh> pobj = InOutToMesh(filename,mesh);
    return pobj;
}

std::shared_ptr<Mesh> InOutToMesh(std::string Filename, stl_reader::StlMesh<float, unsigned int> &mesh){

    std::shared_ptr<Mesh> pmeshout(new Mesh(Filename, mesh.num_tris()+50));



    for(int isolid = 0; isolid < 1; ++isolid) {

        for(size_t itri = mesh.solid_tris_begin(isolid);
            itri < mesh.solid_tris_end(isolid); ++itri)
        {
            const float *c1= mesh.tri_corner_coords(itri,0);
            const float *c2= mesh.tri_corner_coords(itri,1);
            const float *c3= mesh.tri_corner_coords(itri,2);


           Point* p0 = pmeshout->CreateVertex(c1[0],c1[1],c1[2]);
           Point* p1 = pmeshout->CreateVertex(c2[0],c2[1],c2[2]);
           Point* p2 = pmeshout->CreateVertex(c3[0],c3[1],c3[2]);

           if(*p0!=*p1 && *p1!=*p2 && *p2!=*p0){
              pmeshout->CreateTriangle(p0,p1,p2,true);
           }

        }

    }
    return pmeshout;
}