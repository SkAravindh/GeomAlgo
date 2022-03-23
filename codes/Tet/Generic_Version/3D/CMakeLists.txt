set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (Header
        Polygons.h
        Element.h
        Point3D.h
        Mesh3D.h
        Definition.h
        TetIO.h
        Edge3D.h
        Vector_3.h
        Tools.h)

set (Source
        Polygons.cpp
        Element.cpp
        Mesh3D.cpp
        TetIO.cpp)

add_library(3DS STATIC ${Header} ${Source})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
        3DS)
