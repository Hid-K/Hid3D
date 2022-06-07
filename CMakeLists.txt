cmake_minimum_required(VERSION 3.0.0)
project(Hid3D VERSION 0.1.0)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Hid3D main.c)

include_directories( VectorMath )
include_directories( RGB )
include_directories( 3D )

add_library(Vec_LIBRARY STATIC
  VectorMath/Vec3/Vec3.h
  VectorMath/Vec3/Vec3.c
  VectorMath/Vec2/Vec2.h
  VectorMath/Vec2/Vec2.c
  VectorMath/Vec.h
)

add_library(RGB_LIBRARY STATIC
  RGB/RGB.h
  RGB/RGB.c
)

add_library(3D_LIBRARY STATIC
  3D/3D.h
  3D/3D.c
)

add_library(MultithreadComputing_LIBRARY STATIC
  MultithreadComputing/MultithreadComputing.h
  MultithreadComputing/MultithreadComputing.c
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} RGB_LIBRARY)
target_link_libraries(${PROJECT_NAME} MultithreadComputing_LIBRARY)
target_link_libraries(${PROJECT_NAME} Vec_LIBRARY)
target_link_libraries(${PROJECT_NAME} 3D_LIBRARY)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
