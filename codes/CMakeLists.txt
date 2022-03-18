

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC) 
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

#add_subdirectory(Tet)

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
    Rtree.h
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
    LoopEdges.cpp
    )
    
set (Algorithm_header
    Algorithms/MidpointSubdivision.h
    Algorithms/LaplacianSmoothing.h
    Algorithms/AdaptiveRefinement_EdgeBased.h
    Algorithms/SelectiveRefinement.h
    Algorithms/HoleDetection.h
    Algorithms/ExtractIntTriangle.h
    #Algorithms/ConvexHull_3D.cpp
    )
        
set (Algorithm_Source
    Algorithms/MidpointSubdivision.cpp
    Algorithms/LaplacianSmoothing.cpp
    Algorithms/AdaptiveRefinement_EdgeBased.cpp
    Algorithms/SelectiveRefinement.cpp
    Algorithms/HoleDetection.cpp
    Algorithms/SelectiveRefinement_Version2.cpp
    Algorithms/ExtractIntTriangle.cpp
   # Algorithms/ConvexHull_3D.cpp
    )
    

     
set (samples
   # old_files/adaptdesire_version2.cpp
    old_files/adaptdesire_version2_Send.cpp
    old_files/adaptdesire_version2_1_Send.cpp
    old_files/PlaneClipping.cpp
   # old_files/FluidDomain.cpp
     )

set (Header_3D
        Tet/IOtet.h
        Tet/Mesh3D.h
        Tet/Polygon3D.h
        Tet/Point3D.h
        Tet/Definition.h
        Tet/Edge3D.h
        Tet/Face3D.h
        Tet/Elements.h
        )

set (Source_3D
        Tet/IOtet.cpp
        Tet/Mesh3D.cpp
        Tet/Polygon3D.cpp
        Tet/Elements.cpp
        )

FIND_PACKAGE(Boost 1.65 REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

set (Openmesh_Include  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Openmesh/headers)
set (Openmesh_Binaries ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Openmesh/lib)
set (GoogleTest_Include /home/aravindhkumar.kalimuthu/googleTest/googletest/googletest/include/)
set (GoogleTest_Binaries /home/aravindhkumar.kalimuthu/googleTest/googletest/build/lib)
include_directories(${Openmesh_Include})
set (OpenMesh_lib   ${Openmesh_Binaries}/libOpenMeshTools.so
                    ${Openmesh_Binaries}/libOpenMeshTools.so.9.0
                    ${Openmesh_Binaries}/libOpenMeshCore.so
                    ${Openmesh_Binaries}/libOpenMeshCore.so.9.0
        )
include_directories(${GoogleTest_Include})
set (GoogleTest_lib ${GoogleTest_Binaries}/libgmock.a
                    ${GoogleTest_Binaries}/libgmock_main.a
                    ${GoogleTest_Binaries}/libgtest.a
                    ${GoogleTest_Binaries}/libgtest_main.a
        )

add_library(Algos STATIC ${samples} ${Algorithm_header} ${Algorithm_Source}  )

add_library(Geom_Algo STATIC ${Hpp} ${Cpp} )

add_library(Mesh_3D STATIC ${Header_3D} ${Source_3D})

add_executable(${PROJECT_NAME} gTest.cpp) #main.cpp

target_link_libraries(${PROJECT_NAME}
                        Geom_Algo
                        Algos
                        Mesh_3D
                        ${OpenMesh_lib}
                        ${GoogleTest_lib}
                        ${Boost_LIBRARIES}
                        pthread
                        )
                        
#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}) ${PROJECT_BINARY_DIR}--> path to the build directory

 
 

