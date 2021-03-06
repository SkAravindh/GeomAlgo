#ifndef MIDPOINTSUBDIVISION_H
#define MMIDPOINTSUBDIVISION_H
#include <iostream>
#include "../Mesh.h"
#include "../Geom_Support.h"
#include "../Writedata.h"

namespace EdgeMidPoint {

    static int RefineLevel;
    static std::vector<Triangle *> TV;
    static bool forCompleteMesh = false;

    class MidSubdivision {

        public:
            MidSubdivision() = default;
            explicit MidSubdivision(std::shared_ptr<Mesh> &Mesh);
            MidSubdivision(Triangle *T, int C_level, std::shared_ptr<Mesh> &Mesh);
            void loadParameters(Triangle *TInput, Point *PInput, int Refinement_Level);
            void run();
            void getSubdividedTriangles();
            void runForCompleteMesh();
            // void collectallMSDtriangles( std::vector<Triangle *> &STV);
            // Triangle* getParent();

        private:
            std::shared_ptr<Mesh> pMesh;
            int level;
            Triangle *parentT = nullptr;

         protected:
        //Iterators;
            typedef std::vector<Triangle *>::iterator TI;
            typedef std::vector<MidSubdivision *>::iterator MSI;
    };

}


namespace FaceMidPoint {



    class Subdivision {

        public:
            Subdivision() = default;
            explicit Subdivision(std::shared_ptr<Mesh> &mesh);
            void loadParameter(Triangle *TInput, Point *PInput, int Refinement_Level);
            void divideIntoThree();
            void run();

    private:
        std::shared_ptr<Mesh> pMesh;
        int RefineLevel;
        std::vector<Triangle *> TV;
        bool forCompleteMesh = false;
    };
}
#endif