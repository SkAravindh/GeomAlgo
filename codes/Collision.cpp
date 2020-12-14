#include "Collision.h"


bool MTRayTriangleIntersection(vector3 rayOrigin, vector3 rayVector, Triangle *T, vector3 & Intersecting_Point ){

    const float EPSILON = 0.0000001;
    Point *p0 = T->getCorners(0);
    Point *p1 = T->getCorners(1);
    Point *p2 = T->getCorners(2);

    vector3 vertex0(p0->x(),p0->y(),p0->z());
    vector3 vertex1(p1->x(),p1->y(),p1->z());
    vector3 vertex2(p2->x(),p2->y(),p2->z());


    vector3 edge_1, edge_2, h, s, q ;
    double a , f, u, v;

    //Scalar Triple Product to calculate determinant.
    edge_1 = vertex1 - vertex0;
    edge_2 = vertex2 - vertex0;
    h  = cross_product(rayVector,edge_2);
    a  = dot(h,edge_1);

    if (a > -EPSILON && a < EPSILON) {
        return false; //ray parallel to triangle or lies on the plane.
    }

    f = 1/a; //inverse of a determinant

    //Scalar Triple Product to calculate u
    s = rayOrigin - vertex0;
    u = dot(s,h) * f;  //braycentric coordinate.
    if (u < 0.0 || u > 1.0) {
        return false;     //returns false due to property of braycentric coordinate.
    }

    //Scalar Triple Product to calculate v
    q = cross_product(s,edge_1);
    v = dot(rayVector,q) * f;
    if (v < 0.0 || u + v > 1.0) {
        return false;   //returns false due to property of braycentric coordinate.
    }

    // At this stage we can compute t to find out where the intersection point is on the line.
    double t = f * dot(q,edge_2);
    if (t > EPSILON) // ray intersection
    {
        Intersecting_Point = rayOrigin + (rayVector * t);
        return true;
    }
    else {
        return false;  //there is an intersection in plane , not on triangle.
    }
}

bool RayTriangleIntersection(vector3 rayOrigin, vector3 rayVector, Triangle *T, vector3 & Intersecting_Point ){
    Ray camera;
    Plane_3 plane;
    camera.startpoint   = rayOrigin;
    camera.rayDirection = rayVector;
    camera.t  = -1;

    Point *p0 = T->getCorners(0);
    Point *p1 = T->getCorners(1);
    Point *p2 = T->getCorners(2);

    vector3 vertex0(p0->x(),p0->y(),p0->z());
    vector3 vertex1(p1->x(),p1->y(),p1->z());
    vector3 vertex2(p2->x(),p2->y(),p2->z());

    vector3 edge1 = vertex1-vertex0;
    vector3 edge2 = vertex2-vertex0;

    vector3 flat_normal = cross_product(edge1,edge2);
    plane.normal = flat_normal;
    plane.p      = vertex0;

    // calculating T value when ray hits the plane.
    const float EPSILON = 0.00001;
    double n_dot_d = dot(plane.normal, camera.rayDirection);
    // check for denominator zero.
    if(abs(n_dot_d) < 0.00001){
        return false; // which means ray parallel to plane.
    }

    vector3 p_minus_s =  (plane.p-camera.startpoint);
    double n_dot_ps   = dot(plane.normal, p_minus_s);
    camera.t = n_dot_ps/n_dot_d;

    Intersecting_Point  = camera.startpoint + (camera.rayDirection*camera.t);
    vector3 Plane_Point = Intersecting_Point;

    // Is Intersecting_Point Inside  Triangle.

    vector3 AtoB = vertex1-vertex0;
    vector3 BtoC = vertex2-vertex1;
    vector3 CtoA = vertex0-vertex2;

    vector3 AtoPlane_Point = Plane_Point - vertex0;
    vector3 BtoPlane_Point = Plane_Point - vertex1;
    vector3 CtoPlane_Point = Plane_Point - vertex2;

    vector3 Atestvec = cross_product(AtoB , AtoPlane_Point);
    vector3 Btestvec = cross_product(BtoC , BtoPlane_Point);
    vector3 Ctestvec = cross_product(CtoA , CtoPlane_Point);

    bool Atestvec_Dot_Normal = dot(Atestvec, flat_normal) > 0.f;
    if (!Atestvec_Dot_Normal) {
        return false;  //returns false braycentric is negative.
    }

    bool Btestvec_Dot_Normal = dot(Btestvec, flat_normal) > 0.f;
    if (!Btestvec_Dot_Normal) {
        return false;  //returns false braycentric is negative.
    }

    bool Ctestvec_Dot_Normal = dot(Ctestvec, flat_normal) > 0.f;
    if (!Ctestvec_Dot_Normal) {
        return false;  //returns false braycentric is negative.
    }

    bool hit_result = (Atestvec_Dot_Normal && Btestvec_Dot_Normal && Ctestvec_Dot_Normal);
    return hit_result;

}

bool TriangleTriangleIntersection(Triangle *T1,Triangle *T2){

    const float EPSILON = 0.000001;

    Point *p0 = T1->getCorners(0);
    Point *p1 = T1->getCorners(1);
    Point *p2 = T1->getCorners(2);

    Point *q0 = T2->getCorners(0);
    Point *q1 = T2->getCorners(1);
    Point *q2 = T2->getCorners(2);

    Vector3 v0(p0->x(),p0->y(),p0->z());
    Vector3 v1(p1->x(),p1->y(),p1->z());
    Vector3 v2(p2->x(),p2->y(),p2->z());

    Vector3 u0(q0->x(),q0->y(),q0->z());
    Vector3 u1(q1->x(),q1->y(),q1->z());
    Vector3 u2(q2->x(),q2->y(),q2->z());

    Vector3 edge1T1 = v1-v0;
    Vector3 edge2T1 = v2-v0;

    Vector3 edge1T2 = u1-u0;
    Vector3 edge2T2 = u2-u0;

    vector3 N1 = cross_product(edge1T1,edge2T1);
    vector3 N2 = cross_product(edge1T2,edge2T2);


    double du0 =  dot(N1, u0-v0);
    double du1 =  dot(N1, u1-v0);
    double du2 =  dot(N1, u2-v0);

    double du0du1 = du0*du1;
    double du0du2 = du0*du2;

    if (du0du1 > 0.0f && du0du2 > 0.0f) {
        //return false;   //same sign which means triangle two is located other side of Triangle 1.
    }

    double dv0 =  dot(N2, v0-u0);
    double dv1 =  dot(N2, v1-u0);
    double dv2 =  dot(N2, v2-u0);

    double dv0dv1 = dv0*dv1;
    double dv0dv2 = dv0*dv2;

    if (dv0dv1 > 0.0f && dv0dv2 > 0.0f) {
      //  return false;  //same sign which means triangle one is located other side of Triangle 2.
    }

    //check for plane intersection and get line L.
    vector3 D = cross_product(N1,N2);  //Intersecting line
    int index;
    getMaximumIndex(D,index);
    
    //simplified projection of T1 on To L
    double vp0 =v0[index];
    double vp1 =v1[index];
    double vp2 =v2[index];

    //simplified projection of T2 on To L

    double up0 =u0[index];
    double up1 =u1[index];
    double up2 =u2[index];

    //compute Intervals for triangle one
    std::vector<double> Intersects_T1;
    computeInterval(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,Intersects_T1);

    //compute Intervals for triangle two
    std::vector<double> Intersects_T2;
    computeInterval(up0,up1,up2,du0,du1,du2,du0du1,du0du2,Intersects_T2);

    sort(Intersects_T1.begin(), Intersects_T1.end());
    sort(Intersects_T2.begin(), Intersects_T2.end());
    if(Intersects_T1[1] < Intersects_T2[0] || Intersects_T2[1] < Intersects_T1[0]) {
        return false;
    }
    return true;
}


void getMaximumIndex(vector3 &a, int &index){
    double max = a[0];
    index = 0;
    double b = a[1];
    double c = a[2];
    if (b>max) {
        max   = b;
        index = 1;
    }
    if (c>max) {
        max = c;
        index =2;
    }
}

void computeInterval(double PV0,double PV1,double PV2, double D0,double D1,double D2,double d0d1,double d0d2, std::vector<double> &t){
         //PV0,PV1,PV2 projection of vertex on L.
         //D0,D1,D2 projection of triangle  vertices on to another triangle plane.
         //I.E Triangle one vertices on to triangle 2 and vice versa.
         //d0d1, d0d2 defines the vertices position.
    if (d0d1 > 0) {
        //vertex v0 and v1 on the same side whereas v2 on the other side.
        double t1 = computeScalarT(PV0,PV2,D0,D2);
        double t2 = computeScalarT(PV1,PV2,D1,D2);
        t.push_back(t1);
        t.push_back(t2);
    }
    else if (d0d2 > 0) {
        //vertex v0 and v2 on the same side whereas v1 on the other side.
        double t1 = computeScalarT(PV0,PV1,D0,D1);
        double t2 = computeScalarT(PV2,PV1,D2,D1);
        t.push_back(t1);
        t.push_back(t2);
    }
    else if (D1*D2>0 || D0!=0) {
        //vertex v1 and v2 on the same side whereas v0 on the other side.
        double t1 = computeScalarT(PV1,PV0,D1,D0);
        double t2 = computeScalarT(PV2,PV0,D2,D0);
        t.push_back(t1);
        t.push_back(t2);
    }
    else if (D1!=0) {
        double t1 = computeScalarT(PV0,PV1,D0,D1);
        double t2 = computeScalarT(PV2,PV1,D2,D1);
        t.push_back(t1);
        t.push_back(t2);
    }
    else if (D2!=0) {
        double t1 = computeScalarT(PV0,PV2,D0,D2);
        double t2 = computeScalarT(PV1,PV2,D1,D2);
        t.push_back(t1);
        t.push_back(t2);
    }

}

double computeScalarT(double PV0,double PV1, double D0,double D1){

   double t = PV0 + (PV1-PV0)*(D0/(D0-D1));
   return t;
}


void GetMinMax(double po,double p1,double p2, double &pmin, double &pmax){
    double temp_min = std::min(po,p1);
    pmin = std::min(temp_min, p2);
    double temp_max = std::max(po,p1);
    pmax = std::max(temp_max, p2);
}

void GetMinMax(double &xmin, double &ymin, double &zmin, double &xmax, double &ymax, double &zmax, std::vector<vector3> Vertex_RE){
    vector3 vec0 = Vertex_RE[0];
    vector3 vec1 = Vertex_RE[1];
    vector3 vec2 = Vertex_RE[2];

    double temp_minx = std::min(vec0.x(), vec1.x());
    xmin = std::min(temp_minx, vec2.x());

    double temp_miny = std::min(vec0.y(), vec1.y());
    ymin = std::min(temp_miny, vec2.y());

    double temp_minz = std::min(vec0.z(), vec1.z());
    zmin = std::min(temp_minz, vec2.z());

    double temp_maxx = std::max(vec0.x(), vec1.x());
    xmax = std::max(temp_maxx, vec2.x());

    double temp_maxy = std::max(vec0.y(), vec1.y());
    ymax = std::max(temp_maxy, vec2.y());

    double temp_maxz = std::max(vec0.z(), vec1.z());
    zmax = std::max(temp_maxz, vec2.z());

}


bool PerformanceTest(vector3 axis, std::vector<vector3> vertex, std::vector<double> exd){
    double p0 = dot(axis, vertex[0]);
    double p1 = dot(axis, vertex[1]);
    double p2 = dot(axis, vertex[2]);
    double r = ((exd[0] * abs(axis.x())) + (exd[1] * abs(axis.y()))+ (exd[2] * abs(axis.z())) );
    double p_min;
    double p_max;
    GetMinMax(p0,p1,p2, p_min, p_max);
    if ( (p_min > r) || (p_max < -r) ) {
        return false;
    }
    else {
        return true;
    }
}


bool TriangleBoxIntersection(Triangle *T, Bbox_3 &box) {

    std::vector<vector3> Enormals; //(e0,e1,e2)
    std::vector<vector3> Fnormals; //(f0,f1,f2)
    std::vector<vector3> Vertex_relative; //(V0,V1,V2)
    std::vector<double> Extend; //(ex,ey,ez);
    std::vector<vector3> Nine_axis;

    //Calculate Centre
    double cx = (box.xmin()+box.xmax())/2;
    double cy = (box.ymin()+box.ymax())/2;
    double cz = (box.zmin()+box.zmax())/2;
    vector3 centre= vector3(cx,cy,cz);

    //Calculate Extent
    Extend.push_back(abs((box.xmax()-box.xmin())/2));
    Extend.push_back(abs((box.ymax()-box.ymin())/2));
    Extend.push_back(abs((box.zmax()-box.zmin())/2));

    //Collecting Vertex
    Point* Get_V1 = T->getCorners(0);
    Point* Get_V2 = T->getCorners(1);
    Point* Get_V3 = T->getCorners(2);

    vector3 Vertex0 = vector3(Get_V1->x(),Get_V1->y(),Get_V1->z());
    vector3 Vertex1 = vector3(Get_V2->x(),Get_V2->y(),Get_V2->z());
    vector3 Vertex2 = vector3(Get_V3->x(),Get_V3->y(),Get_V3->z());

    vector3 V0 = Vertex0-centre;
    vector3 V1 = Vertex1-centre;
    vector3 V2 = Vertex2-centre;

    Vertex_relative.push_back(Vertex0-centre);
    Vertex_relative.push_back(Vertex1-centre);
    Vertex_relative.push_back(Vertex2-centre);

    //Normal vectors of cube
    Enormals.emplace_back(vector3(1.0,0.0,0.0));
    Enormals.emplace_back(vector3(0.0,1.0,0.0));
    Enormals.emplace_back(vector3(0.0,0.0,1.0));

    Fnormals.push_back(V1-V0);
    Fnormals.push_back(V2-V1);
    Fnormals.push_back(V0-V2);

//Calculating 9 aixs by taking cross product btw triangle edge normal and cube normals
    for(int E=0; E<3; E++){
        for(int F=0; F<3; F++){
            vector3 axis=cross_product(Enormals[E],Fnormals[F]);
            Nine_axis.push_back(axis);
        }
    }

    //Test axis a00 (Projecting triagle vertices and cube centre on plane Nine plane)
    bool variable;
    for(int q=0; q<Nine_axis.size(); q++){
        //std::cout<<"axis "<<q<<std::endl;
        variable= PerformanceTest(Nine_axis[q], Vertex_relative,Extend);
        if(!variable){
            //	std::cout<<"axis failed "<<q<<std::endl;
            break;
        }
    }
    if(!variable){
        return false;
    }

//Test the three planes formed by triangle corresponding to the voxel face normals.
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;
    GetMinMax(xmin, ymin, zmin, xmax, ymax, zmax, Vertex_relative);
    if((xmax < -Extend[0]) || (xmin > Extend[0])){
        return false;
    }
    if((ymax < -Extend[1]) || (ymin > Extend[1])){
        return false;
    }

    if((zmax < -Extend[2]) || (zmin > Extend[2])){
        return false;
    }

// Test separating axis corresponding to triangle face normal
    vector3 Triangle_Normal = cross_product(Fnormals[1],Fnormals[0]);
    Triangle_Normal.make_unit_vector();
    double pl_constant = dot(Triangle_Normal, Vertex0);
    double tri_radi=( (Extend[0] * abs(Triangle_Normal.x())) + (Extend[1] * abs(Triangle_Normal.y())) + (Extend[2] * abs(Triangle_Normal.z())) );
    double var = dot(Triangle_Normal, centre) - pl_constant;
    if(abs(var)>=tri_radi){
        return false;
    }

    return true;
}