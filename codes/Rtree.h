#include "Point.h"
#include "Bbox_3.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>

//written by referring Ranjeeth Mahankali.

constexpr unsigned int rtree_num_element_pernode=16;
namespace bg        = boost::geometry;
namespace bgi       = boost::geometry::index;
namespace bgm       = boost::geometry::model;
namespace rtree     = bgi::detail::rtree;

template<class BoostPoint>
class Rtree {
public:
    typedef BoostPoint                      PointType;
    typedef bgm::box<BoostPoint>            BoxType;
    typedef std::pair<BoxType,size_t>       ItemType;
    typedef bgi::rtree<ItemType, bgi::quadratic<rtree_num_element_pernode>> BoostTreeType;
public:
    void insert(const Bbox_3 &b, size_t i){
        rtree.insert(std::make_pair(toboost(b),i));
    }

    static BoostPoint toboost(const Point* p) {
        return PointType(p->x(),p->y(),p->z());
    }

    static BoxType toboost(const Bbox_3 & b) {
        PointType min = PointType(b.xmin(),b.ymin(),b.zmin());
        PointType max = PointType(b.xmax(),b.ymax(),b.zmax());
        return BoxType (min,max);
    }

    static Point* fromboost(const BoostPoint &a) {
       Point* obj = new Point(bg::get<0>(a), bg::get<1>(a), bg::get<2>(a));
       return obj;
    }

    static Bbox_3 fromboost(const BoxType &b) {

        Point* min = fromboost(b.min_corner());
        Point* max = fromboost(b.max_corner());
        return Bbox_3(min->x(),min->y(),min->z(),max->x(),max->y(),max->z());
    }

    template <typename SizeIter>
    void queryNearest(const Point* p, size_t numResults, SizeIter inserter) const {
        query(bgi::nearest(toboost(p), (unsigned int)numResults), inserter);
    }

    template <typename SizeIter>
    void queryBoxIntersects(const Bbox_3 &b, SizeIter inserter) {
        query(bgi::intersects(toboost(b)), inserter);
    }

    template<typename prediacate_type, typename sizeiter>
    void query(prediacate_type pred, sizeiter inserter) const {
        for (auto i = rtree.qbegin(pred); i != rtree.qend(); i++) {
            *(inserter++) = i->second;
        }
    }

    void clear() {
        rtree.clear();
    }

private:
    BoostTreeType rtree;
};

template class Rtree <bgm::point<float,3,bg::cs::cartesian>>;
typedef Rtree<bgm::point<float,3,bg::cs::cartesian>> Rtree3d;