#include "Geom_Support.h"


Vector3 getNormal(const Triangle *t) {

    Point *p0 = t->getCorners(0);
    Point *p1 = t->getCorners(1);
    Point *p2 = t->getCorners(2);

    Vector3 vertex0 = Vector3(p0->x(), p0->y(), p0->z());
    Vector3 vertex1 = Vector3(p1->x(), p1->y(), p1->z());
    Vector3 vertex2 = Vector3(p2->x(), p2->y(), p2->z());

    Vector3 edge_1 = vertex1 - vertex0;
    Vector3 edge_2 = vertex2 - vertex0;

    Vector3 normal = cross_product(edge_1,edge_2);

    return normal;
}

double getShortestDistance( const Triangle *t, Point *p){

    Point *v0 = t->getCorners(0);
    Vector3 point_on_plane(v0->x(),v0->y(),v0->z());
    Vector3 prj_point(p->x(),p->y(),p->z());
    Vector3 prjP_minus_PP = prj_point - point_on_plane;
    Vector3 normal  = getNormal(t);
    normal.make_unit_vector();
    double distance = dot(normal, prjP_minus_PP);

    return abs(distance);

}

Vector3 getOrthogonalProjectionVector( const Triangle *t, Point *p){

    Point *v0 = t->getCorners(0);
    Vector3 point_on_plane(v0->x(),v0->y(),v0->z());
    Vector3 prj_point(p->x(),p->y(),p->z());
    Vector3 prjP_minus_PP = prj_point - point_on_plane; //K
    Vector3 normal  = getNormal(t);
    normal.make_unit_vector();
    double distance = dot(normal, prjP_minus_PP);
    Vector3 distance_vector = distance*normal;  //N
    //U+N=k ----> u=K-N
    Vector3 projection_vector = prjP_minus_PP-distance_vector;

    return projection_vector;

}

int indexOrder_1(int n){
    int index[]={1,2,0};
    return index[n];

}

int indexOrder_2(int n){
    int index[]={2,0,1};
    return index[n];
}

bool brayCentric(const Triangle *t, Point *p){
    Point *p0 = t->getCorners(0);
    Point *p1 = t->getCorners(1);
    Point *p2 = t->getCorners(2);


    Vector3 vertex0 = Vector3(p0->x(), p0->y(), p0->z());
    Vector3 vertex1 = Vector3(p1->x(), p1->y(), p1->z());
    Vector3 vertex2 = Vector3(p2->x(), p2->y(), p2->z());
    Vector3 P(p->x(),p->y(),p->z());

    Vector3 u = vertex1-vertex0;
    Vector3 v = vertex2-vertex0;
    Vector3 w = P-vertex0;

    Vector3 n = cross_product(u,v);
    double area = 1/(dot(n,n));
    double gamma = ( dot( cross_product(u,w), n ) ) * area;
    double beta  = ( dot( cross_product(w,v), n ) ) * area;
    double alpha = 1-gamma-beta;

    //projection point on to the plane.
    Vector3 Prj_p0 = alpha*vertex0;
    Vector3 Prj_p1 = beta*vertex1;
    Vector3 Prj_p2 = gamma*vertex2;
    Prj_p0+Prj_p1;
    Prj_p0+Prj_p2;
    Vector3 projection_point = Prj_p0;

    bool is_inside = ( (0 <= alpha) && (alpha <= 1) && (0 <= beta)  && (beta  <= 1) && (0 <= gamma) && (gamma <= 1) );

    return is_inside;

}

bool checkHalfEdge(const Triangle *t1, const Triangle *t2){

    for(int i=0; i<3; i++){
        Point *p = t1->getCorners(i);
        for(int j=0; j<3; j++){
            Point *p1 = t2->getCorners(j);
            if(*p==*p1){
                int T1next = indexOrder_1(i);
                int T1prev = indexOrder_2(i);
                int T2next = indexOrder_1(j);
                int T2prev = indexOrder_2(j);
                if(*t1->getCorners(T1next) == *t2->getCorners(T2prev)) return true;
                if(*t1->getCorners(T1prev) == *t2->getCorners(T2next)) return true;
            }
        }
    }
    return false;
}

bool checkCommonVertex(const Triangle *t1, const Triangle *t2){

    for(int i=0; i<3; i++){
        Point *p = t1->getCorners(i);
        for(int j=0; j<3; j++){
            Point *p1 = t2->getCorners(j);
            if(*p==*p1) return true;
        }
    }
    return false;

}

void edgesByOrder(const std::vector<Triangle* > &TV, const int &deg, std::vector<EdgeOrder> &out){
    std::vector<EdgeOrder> outtemp;
    std::vector<Triangle*>::const_iterator it;
    for(it=TV.begin(); it!=TV.end(); it++){
        for(int j=0; j<3; j++) {
            Triangle *tri(*it);
            Point *p0 = tri->getCorners(indexOrder_1(j));
            Point *p1 = tri->getCorners(indexOrder_2(j));
            outtemp.push_back(EdgeOrder(p0,p1));
        }
    }

    std::sort(outtemp.begin(), outtemp.end());
    std::vector<EdgeOrder>::iterator iter;

    for(iter=outtemp.begin(); iter!=outtemp.end(); iter++){
        EdgeOrder EO = *iter;
        int count=1;
        while(++iter!=outtemp.end() && *iter==EO){
         ++count;
        }
        --iter;
        switch(deg)
        {
            case -1:
                if(count == 1) out.push_back(*iter);
                break;
            case -2:
                if(count == 2) out.push_back(*iter);
                break;
            case -3:
                if(count == 3) out.push_back(*iter);
                break;
            default:
                if(deg==count) out.push_back(*iter);
        }

    }
}

void getBorderPoints(const std::vector<EdgeOrder> &BE, std::vector<Point*> &VP){
    std::vector<Point*> points;
    std::vector<EdgeOrder>::const_iterator it;
    std::vector<Point*>::iterator vit;
    for(it = BE.begin(); it != BE.end(); it++){
        points.push_back(it->p0);
        points.push_back(it->p1);
    }
    std::sort(points.begin(),points.end());
    vit = std::unique(points.begin(),points.end());
    points.resize(std::distance(points.begin(),vit));
    VP.assign(points.begin(), points.end());
}