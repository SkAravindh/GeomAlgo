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

double getShortestDisPointToPlane( const Triangle *t, const Point *p)  {

    Point *v0 = t->getCorners(0);
    Vector3 point_on_plane(v0->x(),v0->y(),v0->z());
    Vector3 prj_point(p->x(),p->y(),p->z());
    Vector3 prjP_minus_PP = prj_point - point_on_plane;
    Vector3 normal  = getNormal(t);
    normal.make_unit_vector();
    double distance = dot(normal, prjP_minus_PP);

    return abs(distance);
}

Vector3 getOrthogonalProjectionVector( const Triangle *t, const Point *p) {

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

int indexOrder_1(int n) {
    int index[]={1,2,0};
    return index[n];
}

int indexOrder_2(int n) {
    int index[]={2,0,1};
    return index[n];
}

bool brayCentric(const Triangle *t, const Point *p, Vector3* projected_point) {

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
    double Inv_area = 1/(dot(n,n));
    double gamma = ( dot( cross_product(u,w), n ) ) * Inv_area;
    double beta  = ( dot( cross_product(w,v), n ) ) * Inv_area;
    double alpha = 1-gamma-beta;

    //projection point on to the plane.
    Vector3 Prj_p0 = alpha*vertex0;
    Vector3 Prj_p1 = beta*vertex1;
    Vector3 Prj_p2 = gamma*vertex2;
    Prj_p0+Prj_p1;
    Prj_p0+Prj_p2;
    Vector3 projectedPoint = Prj_p0;
    *projected_point = Prj_p0;
    std::cout << "projectedPoint "<<projectedPoint<<std::endl;
    bool is_inside = ( (0 <= alpha) && (alpha <= 1) && (0 <= beta)  && (beta  <= 1) && (0 <= gamma) && (gamma <= 1) );

    return is_inside;
}

bool checkForHalfEdge(const Triangle *t1, const Triangle *t2) {

    for(int i=0; i<3; i++) {
        Point *p = t1->getCorners(i);
        for(int j=0; j<3; j++) {
            Point *p1 = t2->getCorners(j);
            if(*p==*p1) {
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

double getDistance(const Point* p0, const Point* p1) {
    return (std::sqrt( std::pow((p1->x()-p0->x()),2) + std::pow((p1->y()-p0->y()),2) + std::pow((p1->z()-p0->z()),2)) );
}

bool checkCommonVertex(const Triangle *t1, const Triangle *t2){

    for(int i=0; i<3; i++) {
        Point *p = t1->getCorners(i);
        for(int j=0; j<3; j++) {
            Point *p1 = t2->getCorners(j);
            if(*p==*p1) return true;
        }
    }
    return false;
}

void getedgesByOrder(const std::vector<Triangle* > &TV, const int &deg, std::vector<EdgeOrder> &out) {

    std::vector<EdgeOrder> outtemp;
    std::vector<Triangle*>::const_iterator it;
    for(it=TV.begin(); it!=TV.end(); it++) {
        for(int j=0; j<3; j++) {
            Triangle *tri(*it);
            Point *p0 = tri->getCorners(indexOrder_1(j));
            Point *p1 = tri->getCorners(indexOrder_2(j));
            outtemp.push_back(EdgeOrder(p0,p1));
        }
    }

    std::sort(outtemp.begin(), outtemp.end());
    std::vector<EdgeOrder>::iterator iter;

    for(iter=outtemp.begin(); iter!=outtemp.end(); iter++) {
        EdgeOrder EO = *iter;
        int count=1;
        while(++iter!=outtemp.end() && *iter==EO) {
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

void getBorderPoints(const std::vector<EdgeOrder> &BE, std::vector<Point*> &VP) {

    std::vector<Point*> points;
    std::vector<EdgeOrder>::const_iterator it;
    std::vector<Point*>::iterator vit;
    for(it = BE.begin(); it != BE.end(); it++) {
        points.push_back(it->p0);
        points.push_back(it->p1);
    }
    std::sort(points.begin(),points.end());
    vit = std::unique(points.begin(),points.end());
    points.resize(std::distance(points.begin(),vit));
    VP.assign(points.begin(), points.end());
}

void getEdgesofTrianlges( const std::vector<Triangle* > &TV, std::vector<EdgeOrder> &BE) {

    std::set<EdgeOrder> edge;
    for(auto triangle : TV) {
        Triangle* CT(triangle);
        for (int i=0; i<3; i++) {
            EdgeOrder ed = CT->getEO(i);
            edge.insert(ed);
        }
    }
    BE.assign(edge.begin(),edge.end());
}

void eraseCertainTriangle(std::vector<Triangle* > &TV, Triangle *t) {
    TV.erase(std::remove(TV.begin(), TV.end(), t), TV.end());
}

void eraseCertainEntryPT(std::multimap<Point*, Triangle*,ComparePoint>&mm, Point *key, Triangle* t) {

    typedef std::multimap<Point*, Triangle*,ComparePoint>::iterator MMPT;
    std::pair<MMPT, MMPT> point_triangle = mm.equal_range(key);
    for(MMPT iter = point_triangle.first; iter != point_triangle.second; iter++ ) {
        Point *p = iter->first;
        Triangle *tri = iter->second;
        if( *p == *key && *tri == *t) {
            mm.erase(iter);
            break;
        }
    }
}

void eraseCertainEntryET(std::multimap<EdgeOrder, Triangle*>&mm, EdgeOrder &key, Triangle* t) {

    typedef std::multimap<EdgeOrder, Triangle *>::iterator MMET;
    std::pair<MMET, MMET> edge_triangle = mm.equal_range(key);
    for(MMET iter = edge_triangle.first; iter != edge_triangle.second; iter++ ) {
        EdgeOrder eo = iter->first;
        Triangle *tri = iter->second;
        if( eo == key && *tri == *t) {
            mm.erase(iter);
            break;
        }
    }
}

bool getCommonEdge(Triangle *t1, Triangle *t2,EdgeOrder &oe) {

    std::vector<Triangle*> tv;
    std::map<EdgeOrder, std::vector<Triangle*>> edtoT;
    tv.push_back(t1);
    tv.push_back(t2);
    for(int i=0; i<2; i++) {
        Triangle* current(tv[i]);
        for(int j=0; j<3; j++) {
            EdgeOrder ed = current->getEO(j);
            edtoT[ed].push_back(current);
        }
    }

    for(auto it = edtoT.begin(); it != edtoT.end(); it++ ) {
        if(it->second.size()==2) {
           oe=it->first;
            return true;
        }
    }

    std::cout << "No common edge between those two triangles so returning false !!! " << std::endl;
    return false;
}

int getNonCommonPointsIDs(Triangle *t1, Triangle *t2, int Triangle_no, std::vector<int> &IDV) {
    //works only when triangles have halfedge

    int id1;
    int id2;
    for(int i=0; i<3; i++) {
        Point* p0 = t1->getCorners(i);
        for(int j=0; j<3; j++) {
            Point * p1 = t2->getCorners(j);
            if(*p0 == *p1) {
                int T1next = indexOrder_1(i);
                int T1prev = indexOrder_2(i);
                int T2next = indexOrder_1(j);
                int T2prev = indexOrder_2(j);
                if(*t1->getCorners(T1next) == *t2->getCorners(T2prev)) {
                    id2 = T2next;
                    id1 = T1prev;
                }
                if(*t1->getCorners(T1prev) == *t2->getCorners(T2next)) {
                    id2 = T2prev;
                    id1 = T1next;
                }
            }
        }
    }
    switch (Triangle_no) {
        case 1:
            return id1;
            break;
        case 2:
            return id2;
            break;
        default:
            IDV.push_back(id1);
            IDV.push_back(id2);
            break;
    }
}

double getAngleBtwVectors(const Vector3 &v1, const Vector3 &v2) {

    double vec_dot   =  dot(v1,v2);
    double v1_length = v1.lenght();
    double v2_length = v2.lenght();
    double angle = acos( vec_dot/(v1_length * v2_length) );
   // double degree = (angle * 180) / pi;   //converting radian to degree.
   // std::cout << "angle " << angle << std::endl;
    return angle; //return by radians
}

double getMinimumDistance(const Triangle* t, const Point* p) {

    Vector3* prj_point = new Vector3 ();
    if(brayCentric(t,p,prj_point)) {
        Point* p0 = new Point(prj_point->x(),prj_point->y(),prj_point->z());
        double dis = getDistance(p0,p);
        return dis;
    }
    Vector3 point = Vector3(p->x(), p->y(), p->z());
    double allDistance [3];
    for(int i=0; i<3; i++) {
        Point *p1 = t->getCorners(indexOrder_1(i));
        Point *p2 = t->getCorners(indexOrder_2(i));
        Vector3 v1 = Vector3(p1->x(), p1->y(), p1->z());
        Vector3 v2 = Vector3(p2->x(), p2->y(), p2->z());
        double distance = segmentPointDistance(v1, v2, point);
      //  std::cout<<"i is "<<i<<"distance is " << distance << std::endl;
        allDistance[i] =distance;
    }
    double dummy = DBL_MAX;
    for(int i=0; i<3; i++){
        if(allDistance[i] < dummy ) {
            dummy = allDistance[i];
        }
    }
    return dummy;
}

double segmentPointDistance(Vector3 start, Vector3 end, Vector3 p) {

    Point* start_p1 = new Point(start.x(),start.y(),start.z());
    Point* end_p2 = new Point(end.x(),end.y(),end.z());
    Point* input_point = new Point(p.x(),p.y(),p.z());
    Point* prj_point_dummy= nullptr;
    Vector3 edge_v1v2 = end-start;
    Vector3 edge_v1p  = p-start;
    double distance = getDistance(start_p1,end_p2);
    double is_on_segment = dot(edge_v1v2, edge_v1p)/(distance*distance);

    if(is_on_segment < 0 ) {
        prj_point_dummy= new Point(start.x(),start.y(),start.z());
    }else if(is_on_segment > 0) {
        prj_point_dummy= new Point(end.x(),end.y(),end.z());
    }
    else {
       Vector3 prj_point = (start + is_on_segment*(edge_v1v2));
        prj_point_dummy  = new Point(prj_point.x(),prj_point.y(),prj_point.z());
    }
    return getDistance(prj_point_dummy,input_point);
}