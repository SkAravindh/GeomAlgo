

set (Hpp
    Triangle.h
    Point.h
    EdgeOrder.h
    Mesh.h
    InOut.h
    Collision.h
    Geom_Support.h
    Bbox_3.h
    Writedata.h
    Algorithms/MidpointSubdivision.h
    Algorithms/AlgorithmInitialize.h
    Algorithms/AdaptivieTriangle.h
    )
    
set (Cpp
    Triangle.cpp
    Point.cpp
    EdgeOrder.cpp
    Mesh.cpp
    InOut.cpp
    Collision.cpp
    Geom_Support.cpp
    Bbox_3.cpp
    Writedata.cpp
    Algorithms/MidpointSubdivision.cpp
    Algorithms/AlgorithmInitialize.cpp
    Algorithms/AdaptivieTriangle.cpp
    )

add_library(Geom_Algo STATIC ${Hpp} ${Cpp})


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Geom_Algo)

