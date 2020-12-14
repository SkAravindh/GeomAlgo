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
            )

add_executable(output main.cpp)
target_link_libraries(output Algofiles)

