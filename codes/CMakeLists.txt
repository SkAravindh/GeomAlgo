add_library(
            Algofiles 
            Triangle.h
            Triangle.cpp
            Point.h
            Point.cpp
            EdgeOrder.h
            EdgeOrder.cpp
            Mesh.h
            Mesh.cpp
            InOut.h
            InOut.cpp
            Collision.h
            Collision.cpp
            Geom_Support.h
            Geom_Support.cpp
            Bbox_3.h
            Bbox_3.cpp
            Writedata.h
            Writedata.cpp
            Algorithms/MidpointSubdivision.h
            Algorithms/MidpointSubdivision.cpp
            Algorithms/AlgorithmInitialize.h
            Algorithms/AlgorithmInitialize.cpp
            Algorithms/AdaptivieTriangle.h
            Algorithms/AdaptivieTriangle.cpp
            Algorithms/AdaptParent.cpp
            Algorithms/AdaptParent.h
            )

add_executable(output main.cpp)
target_link_libraries(output Algofiles)

