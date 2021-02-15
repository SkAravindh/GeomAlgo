

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
    Macros.h
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
    )
    
set (Algorithm_header
    Algorithms/MidpointSubdivision.h
    Algorithms/LaplacianSmoothing.h
    Algorithms/AdaptiveRefinement_EdgeBased.h
    Algorithms/SelectiveRefinement.h
    )
    
set (Algorithm_Source
    Algorithms/MidpointSubdivision.cpp
    Algorithms/LaplacianSmoothing.cpp
    Algorithms/AdaptiveRefinement_EdgeBased.cpp
    Algorithms/SelectiveRefinement.cpp
    )
    
set (samples
    verification/test_connectivity.cpp
    verification/red_green_try.cpp
    verification/establishNeigh.cpp
    verification/estnadapt.cpp
    verification/subdivision_EDbased.cpp
    verification/adaptdesirededge.cpp
    verification/adaptdesire_version2.cpp
    verification/adaptdesire_version2_Send.cpp
    verification/looping.cpp
    )
    
add_library(Algos STATIC ${samples} ${Algorithm_header} ${Algorithm_Source})

add_library(Geom_Algo STATIC ${Hpp} ${Cpp} )

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
                        Geom_Algo
                        Algos
                        )
                        
#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}) ${PROJECT_BINARY_DIR}--> path to the build directory

 
 

