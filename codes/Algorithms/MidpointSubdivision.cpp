#include "MidpointSubdivision.h"

namespace EdgeMidPoint {

    EdgeMidPoint::MidSubdivision::MidSubdivision(std::shared_ptr<Mesh> &Mesh) : pMesh(Mesh) {
    }

    EdgeMidPoint::MidSubdivision::MidSubdivision(Triangle *T, int C_level, std::shared_ptr<Mesh> &Mesh) {

        this->level = C_level;

        //Assigning current triangle to parent.
        parentT = T;

        //Return current triangle object as if reach the given level.
        if (level >= RefineLevel) {
            return;
        }

        //Taking Midpoint of Current Triangle to initiate the splitting process.
        Point *edge0mid = parentT->getEdgeMidPoint(0);
        Point *edge1mid = parentT->getEdgeMidPoint(1);
        Point *edge2mid = parentT->getEdgeMidPoint(2);


        //Splitting the parent triangle into four and storeing into alltriangle container(check Mesh.h)
        Mesh->createTriangle(edge0mid, edge1mid, edge2mid, parentT);
        Mesh->createTriangle(edge0mid, parentT->getCorners(2), edge1mid, parentT);
        Mesh->createTriangle(edge1mid, parentT->getCorners(0), edge2mid, parentT);
        Mesh->createTriangle(edge2mid, parentT->getCorners(1), edge0mid, parentT);

        //Storing Triangle for further subdivision.
        std::vector<Triangle *> thisTvec;
        parentT->getChildren(thisTvec);

        //Level Increment
        ++level;

        for (TI it = thisTvec.begin(); it != thisTvec.end(); it++) {
            MidSubdivision *subdi = new MidSubdivision(*it, level, Mesh);
        }
    }

    void EdgeMidPoint::MidSubdivision::loadParameters(Triangle *TInput, Point *PInput, int Refinement_Level) {

        RefineLevel = Refinement_Level;
        std::vector<Triangle *> tv;
        if (TInput != nullptr) {
            pMesh->getNeigTrianglesbyOrder(TInput, 1, tv);
            pMesh->fillTriangleContainers(tv, external);
        } else if (PInput != nullptr) {
            pMesh->getRingNeigbyOrder(PInput, 1, tv);
            pMesh->fillTriangleContainers(tv, external);
        } else {
            std::cout << "Midpoint Subdivision Initialized for complete mesh " << std::endl;
            forCompleteMesh = true;
        }
    }

    void EdgeMidPoint::MidSubdivision::run() {

        if (forCompleteMesh) {
            runForCompleteMesh();
            forCompleteMesh = false;
            return;
        }

        pMesh->getTriangleContainers(TV, external);
        for (auto ele : TV) {
            pMesh->delCertainTriInalltriangles(ele);
        }

        for (int i = 0; i < TV.size(); i++) {
            MidSubdivision *obj = new MidSubdivision(TV[i], 0, pMesh); //use [2] for proper result
        }

    }

    void EdgeMidPoint::MidSubdivision::runForCompleteMesh() {

        pMesh->getTriangles(TV);
        pMesh->clearTV(alltri);

        for (int i = 0; i < TV.size(); i++) {
            MidSubdivision *obj = new MidSubdivision(TV[i], 0, pMesh); //use [2] for proper result
        }

    }

    void EdgeMidPoint::MidSubdivision::getSubdividedTriangles() {

        std::vector<Triangle *> out;
        for (auto ele : TV) {
            ele->getChildren(out);
        }

        pMesh->fillTriangleContainers(out, alltri);
        pMesh->reEstablishConnectivity();
        TV.clear();

    }
}


namespace FaceMidPoint {

    FaceMidPoint::Subdivision::Subdivision(std::shared_ptr<Mesh> &mesh) : pMesh(mesh) {

    }

    void FaceMidPoint::Subdivision::divideIntoThree() {

//        std::vector<Triangle*> allTriangles;
//        pMesh->getTriangles(allTriangles);
//        std::vector<Triangle*> finalT;
//        finalT.push_back(allTriangles[0]);

        std::vector<Triangle*> datastructT;
        for(int i=0; i<RefineLevel; i++) {

            std::vector<Triangle*> tritorefine;
            tritorefine.assign(TV.begin(),TV.end());
            std::cout << "size final t " << TV.size() << std::endl;
            TV.clear();

            while(!tritorefine.empty()) {

                Triangle* currentT = *tritorefine.begin();
                eraseCertainTriangle(tritorefine,currentT);

                Point* centroid = currentT->getCentroid();
                Point* C0 = currentT->getCorners(0);
                Point* C1 = currentT->getCorners(1);
                Point* C2 = currentT->getCorners(2);

                pMesh->createTriangle(C0,C1,centroid,currentT);
                pMesh->createTriangle(centroid,C2,C0,currentT);
                pMesh->createTriangle(centroid,C1,C2,currentT);
                datastructT.push_back(currentT);
                std::vector<Triangle*> children;
                currentT->getChildren(children);

                for (auto ele : children ) {
                    TV.push_back(ele);
                }

            }
        }
        for (auto ele : datastructT) {
            pMesh->delCertainEntryPT(ele);
            pMesh->delCertainEntryET(ele);
            pMesh->delCertainTriInalltriangles(ele);
        }

        std::cout << "final triangle " << TV.size() << std::endl;
        pMesh->fillTriangleContainers(TV,alltri);
    }

    void FaceMidPoint::Subdivision::loadParameter(Triangle *TInput, Point *PInput, int Refinement_Level) {

        RefineLevel = Refinement_Level;

        if (TInput != nullptr) {
            pMesh->getNeigTrianglesbyOrder(TInput, 1, TV);

        } else if (PInput != nullptr) {
            pMesh->getRingNeigbyOrder(PInput, 1, TV);

        } else {
            std::cout << "Face Subdivision Initialized for complete mesh " << std::endl;
            forCompleteMesh = true;
        }
    }

    void FaceMidPoint::Subdivision::run() {

        if (forCompleteMesh) {
            std::vector<Triangle*> alltriangles;
            pMesh->getTriangles(alltriangles);
            TV.assign(alltriangles.begin(),alltriangles.end());
            divideIntoThree();
            forCompleteMesh = false;
            return;
        }
        divideIntoThree();
    }

}



//void MidSubdivision::getSubdividedTriangles() {

//    std::vector<Triangle*> out;
//    for(MSI iter = MidSubVector.begin(); iter!= MidSubVector.end(); iter++){
//        MidSubdivision* C_obj(*iter);
//        C_obj->collectallMSDtriangles(out);
//    }
//    std::cout << "out "<<out.size() << std::endl;

//}

//void MidSubdivision::collectallMSDtriangles( std::vector<Triangle *> &STV) {
//
//    if( Vchild.empty() ) {
//        STV.push_back(this->getParent());
//    }
//    else{
//        //return;
//    }
//
//    //recursively collect all subdivided triangle object.
//    for(MSI iter = Vchild.begin(); iter!= Vchild.end(); iter++) {
//        MidSubdivision* C_vc(*iter);
//        C_vc->collectallMSDtriangles(STV);
//    }
//}

//Triangle* MidSubdivision::getParent() {
//    return parentT;
//}