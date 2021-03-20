cmake_minimum_required(VERSION 3.8)
project(HalfEdge VERSION 1.0.0)

find_package(Boost)
#add_library(HE STATIC 
 #           EdgeHE.cpp
  #          HalfEdge.cpp
   #         FaceHE.cpp
    #        VertexHE.cpp
     #       PointHE.cpp)

add_executable(output main.cpp
                       Mesh.cpp
                        InOut.cpp)
target_link_libraries(output Boost::boost)
