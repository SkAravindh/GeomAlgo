set (Header_3D
        IOtet.h
        Mesh3D.h
        Polygon3D.h
        )

set (Source_3D
        IOtet.cpp
        Mesh3D.cpp
        Polygon3D.cpp
        )

add_library(Mesh_3D STATIC ${Header_3D} ${Source_3D})

target_link_libraries(${PROJECT_NAME}
                         Mesh_3D   )