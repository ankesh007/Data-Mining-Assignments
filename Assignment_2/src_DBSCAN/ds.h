#ifndef DS_H
#define DS_H

#include <bits/stdc++.h>
#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
using namespace std;


namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 1, bg::cs::cartesian> point_1;
typedef bg::model::box<point_1> box_1;
typedef std::pair<box_1, size_t> value_1;

typedef bg::model::point<float, 2, bg::cs::cartesian> point_2;
typedef bg::model::box<point_2> box_2;
typedef std::pair<box_2, size_t> value_2;

typedef bg::model::point<float, 3, bg::cs::cartesian> point_3;
typedef bg::model::box<point_3> box_3;
typedef std::pair<box_3, size_t> value_3;

typedef bg::model::point<float, 4, bg::cs::cartesian> point_4;
typedef bg::model::box<point_4> box_4;
typedef std::pair<box_4, size_t> value_4;

typedef bg::model::point<float, 5, bg::cs::cartesian> point_5;
typedef bg::model::box<point_5> box_5;
typedef std::pair<box_5, size_t> value_5;

void init_ds(int dim,int inst,float eps);
void query_ds(int index,vector<int> &neighbour);

#endif