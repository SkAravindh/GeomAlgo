

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    Edge.h
    Algorithms/MidpointSubdivision.h
    Algorithms/LaplacianSmoothing.h
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
    Edge.cpp
    Writedata.cpp
    test_connectivity.cpp
    Algorithms/MidpointSubdivision.cpp
    Algorithms/LaplacianSmoothing.cpp
    )
    
 


add_library(Geom_Algo STATIC ${Hpp} ${Cpp})


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Geom_Algo)

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}) ${PROJECT_BINARY_DIR}--> path to the build directory

 
 

