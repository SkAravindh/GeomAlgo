#include "Mesh.h"


struct point3 {

    point3() = default;
    point3(double x, double y, double z) {
        coordinates[0] = x;
        coordinates[1] = y;
        coordinates[2] = z;
    }
    double x() const {return coordinates[0];};
    double y() const {return coordinates[1];};
    double z() const {return coordinates[2];};
    bool operator < (const point3 &rhs) const {
        //std::cout<<"hi < "<<std::endl;
        return std::make_tuple(this->x(), this->y(), this->z()) < std::make_tuple(rhs.x(), rhs.y(), rhs.z()) ;
    }
    friend std::ostream& operator << (std::ostream &out , const point3 &rhs) {
        out<<rhs.x() << " "<<rhs.y() << " " << rhs.z();
        return out;
    }
    double coordinates[3];
};

struct Vert {

    Vert() = default;
    Vert(double x, double y, double z) {
        coordinates[0] = int(x*100.);
        coordinates[1] = int(y*100.);
        coordinates[2] = int(z*100.);
    }
    double x() const {return coordinates[0];};
    double y() const {return coordinates[1];};
    double z() const {return coordinates[2];};
    bool operator < (const Vert &rhs) const {
        //std::cout<<"hi < "<<std::endl;
        return std::make_tuple(this->x(), this->y(), this->z()) < std::make_tuple(rhs.x(), rhs.y(), rhs.z()) ;
    }
    friend std::ostream& operator << (std::ostream &out , const Vert &rhs) {
        out<<rhs.x() << " "<<rhs.y() << " " << rhs.z();
        return out;
    }
    double coordinates[3];
};

struct comp_point   {
    bool operator()(const Point* p1,const Point* p2) const noexcept {
        double x = int(p1->x()*100.);
        double y = int(p1->y()*100.);
        double z =  int(p1->z()*100.);

        double x_ = int(p2->x()*100.);
        double y_ = int(p2->y()*100.);
        double z_ =  int(p2->z()*100.);

        return std::make_tuple(x, y, z) < std::make_tuple(x_, y_, z_) ;
    }
};

class compare {
public:
    compare() = default;
    compare(std::shared_ptr<Mesh> &nas, std::shared_ptr<Mesh> &input);
};

compare::compare(std::shared_ptr<Mesh> &nas, std::shared_ptr<Mesh> &input) {
    std::vector<Point*> input_points;
    std::vector<Point*> nas_points;
    input->getVerticesAll(input_points);
    nas->getVerticesAll(nas_points);
    std::sort(input_points.begin(),input_points.end(),comp_point());
    std::sort(nas_points.begin(),nas_points.end(),comp_point());
    for(int i=0; i<nas_points.size(); i++) {
     //   std::cout<<*input_points.at(i)<<" " <<*nas_points.at(i) << std::endl;
    }

    std::vector<Point*> input_points_change;
    std::vector<Point*> nas_points__change;

    std::vector<int> input_points_change_id;
    std::vector<int> nas_points__change_id;

    int count_=0;
    for(int i=0; i<input_points.size(); i++) {

        Point* ptest = input_points.at(i);
        Point* ele = nas_points.at(i);
        double x = std::fabs( ptest->x() - ele->x() );
        double y = std::fabs( ptest->y() - ele->y() );
        double z = std::fabs( ptest->z() - ele->z() );
        if (x <= 0.001 && y <= 0.001 && z <= 0.001) {
            count_ = count_ + 1;
        }
        else {
            input_points_change_id.push_back(i);
            nas_points__change_id.push_back(i);
              input_points_change.push_back(input_points.at(i));
            nas_points__change.push_back(nas_points.at(i));
          //  std::cout<<"i "<<i<<" "<<*input_points.at(i)<<" "<<*nas_points.at(i)<<std::endl;
        }
    }
    std::cout<<"count_ " << count_<<" "<<input_points_change.size()<<" "<<nas_points__change.size()<< std::endl;


    int c=0;

  //  for(int i=0; i<input_points_change.size(); i++) {
    //    for(int j=0; j<nas_points__change.size(); j++) {
    std::map<int,int> input_to_nas;
    std::map<int, Point*> int_to_naspoint;
    for(auto id1: input_points_change_id){
        for(auto id2: nas_points__change_id) {
            Point* ptest_ = input_points.at(id1);
            Point* ele_ = nas_points.at(id2);
            double x = std::fabs( ptest_->x() - ele_->x() );
            double y = std::fabs( ptest_->y() - ele_->y() );
            double z = std::fabs( ptest_->z() - ele_->z() );
            if (x <= 0.001 && y <= 0.001 && z <= 0.001) {
                c=c+1;
                input_to_nas.insert({id1,id2});
                int_to_naspoint.insert({id2,ele_});
             //   std::cout<<"i "<<id1<<" "<<"j "<<id2<<" "<<*input_points.at(id1)<<" "<<*nas_points.at(id2)<<std::endl;
                break;
            }
        }
    }
    std::cout<<"c " << c<< std::endl;
    std::cout<<"input_points "<<input_points.size()<<std::endl;
    if(c+count_ == input_points.size()) {
        std::cout<<"yes they are equal " << std::endl;
    }
    for(auto ele : input_to_nas) {
        Point* p = int_to_naspoint[ele.second];
        nas_points[ele.first] = p;
    }

    int final_c =0;
    for(int i=0; i<nas_points.size(); i++){
        Point* input_p = input_points.at(i);
        Point* nas_p   = nas_points.at(i);
        double x = std::fabs( input_p->x() - nas_p->x() );
        double y = std::fabs( input_p->y() - nas_p->y() );
        double z = std::fabs( input_p->z() - nas_p->z() );
        if (x <= 0.001 && y <= 0.001 && z <= 0.001) {
            final_c=final_c+1;
        }
    }
    std::cout<<"final_c "<<final_c<<" "<<"input_points "<<input_points.size()<<std::endl;
    if(final_c == input_points.size()) {
        std::cout<<"yes they are equal -----------xxxxxxxx---------- " << std::endl;
    }
}

/*compare::compare(std::shared_ptr<Mesh> &nas, std::shared_ptr<Mesh> &input) {

    std::map<Point*,Point*,ComparePoint> nas_to_point;
    std::vector<Point*> input_points;
    input->getVerticesAll(input_points);
    input->computeRtree();
    const Rtree3d &mvertexTree = input->getVertexTree();
      Rtree3d mfaceTree = input->getFaceTree();
    std::vector<Point*> nas_points;
    nas->getVerticesAll(nas_points);


    int count = 0;
    for(auto ele : nas_points) {
        Vector3 origin(0.1,0.1,0.1);
        Vector3 p = to_Vector3(ele);
        Vector3 min = p-origin;
        Vector3 max = p+origin;
     //   Point* min_p = to_Point(min);
     //   Point* max_p = to_Point(max);
        std::vector<size_t> candidates;
        Bbox_3 obj(min.x(),min.y(),min.z(),max.x(),max.y(),max.z());
        mfaceTree.queryBoxIntersects(obj,std::back_inserter(candidates) );
        std::cout<<"candidates "<<candidates.size()<<std::endl;
        for(auto id : candidates) {
            std::cout<<*input_points.at(id)<<std::endl;
        }
        std::cout<<"ele is "<<" "<<*ele<<std::endl;
        break;
    }
}*/

/*compare::compare(std::shared_ptr<Mesh> &nas, std::shared_ptr<Mesh> &input) {

    std::vector<Point*> inputallpoints;
    input->computeRtree();
    input->getVerticesAll(inputallpoints);
    std::cout<<"inputallpoints "<<inputallpoints.size()<<std::endl;
    std::vector<Point*> naspoint;
    nas->getVertices(naspoint);
   size_t id = input->computeclosest(naspoint.at(0));
   std::cout<<"id " << id << std::endl;
    std::cout << *inputallpoints.at(id) <<" "<<"naspoint "<<*naspoint.at(0) << std::endl;
  std::map<Vert,point3> vert_to_point3;
    std::map<Vert,Point*> vert_to_Point_3;
    std::vector<Triangle*> nas_triangle;
    nas->getTriangles(nas_triangle);
    for(Triangle* nas_t : nas_triangle) {
        double x1 = nas_t->getCorners(0)->x();
        double y1 = nas_t->getCorners(0)->y();
        double z1 = nas_t->getCorners(0)->z();

        double x2 = nas_t->getCorners(1)->x();
        double y2 = nas_t->getCorners(1)->y();
        double z2 = nas_t->getCorners(1)->z();

        double x3 = nas_t->getCorners(2)->x();
        double y3 = nas_t->getCorners(2)->y();
        double z3 = nas_t->getCorners(2)->z();

        Vert p0 = Vert(x1,y1,z1);
        Vert p1 = Vert(x2,y2,z2);
        Vert p2 = Vert(x3,y3,z3);

        point3 v0 = point3(x1,y1,z1);
        point3 v1 = point3(x2,y2,z2);
        point3 v2 = point3(x3,y3,z3);

        vert_to_point3.insert({p0,v0});
        vert_to_point3.insert({p1,v1});
        vert_to_point3.insert({p2,v2});
    }
    std::set<Point> setpoints;
    std::vector<Point*> alivepoints;
    input->getVertices(alivepoints);
    for(auto pts: alivepoints) {
        double x = pts->x();
        double y = pts->y();
        double z =  pts->z();
        Vert obj(x,y,z);
        vert_to_Point_3.insert({obj,pts});

    }
    std::vector<Vert> vert;
    std::vector<Vert> Vert_3;
    for(auto ele1 : vert_to_Point_3)
    {
        Vert_3.push_back(ele1.first);
    }
    for(auto ele1 : vert_to_point3)
    {
        vert.push_back(ele1.first);
    }
    for(int i=0; i<Vert_3.size(); i++) {
        std::cout << vert.at(i) << " "<<Vert_3.at(i) << std::endl;
    }

    int nas_m=0;
    std::map<point3,Point*> nas_to_merge;
    for(auto ele : vert_to_point3) {
        nas_m=nas_m+1;
        auto it = vert_to_Point_3.find(ele.first);
        if(it != vert_to_Point_3.end()) {

            nas_to_merge.insert({ele.second,it->second});
        }
        //else {
          //  std::cout<<"else part " << std::endl;
        //}
    }
    for(auto pts : alivepoints) {
        setpoints.insert(*pts);
    }
    int id = 0;
    for(auto ele : nas_to_merge) {
        auto it = setpoints.find(*ele.second);
        if(it != setpoints.end()) {
            id = id+1;
        }
    }

    int count = 0;
    for(auto ele : nas_to_merge) {

        double x = std::fabs( ele.first.x() - ele.second->x() );
        double y = std::fabs( ele.first.y() - ele.second->y() );
        double z = std::fabs( ele.first.z() - ele.second->z() );
        if (x <= 0.001 && y <= 0.001 && z <= 0.001) {
            count = count +1;
        }
        //std::cout<<ele.first << " " << *ele.second << std::endl;
    }
    std::cout<<"vert_to_Point_3 "<<vert_to_Point_3.size()<< " "<<"vert_to_point3 "<<vert_to_point3.size()<<" "<<"id "<<id<<" "<<"Vert_3 "<<Vert_3.size()<<std::endl;
    std::cout<<"nas_to_merge "<<nas_to_merge.size()<<" "<<"count "<<count<<" "<<"alivepoints "<<alivepoints.size()<< std::endl;*/
//}