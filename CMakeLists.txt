CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
FIND_PACKAGE(OpenCV REQUIRED)

PROJECT(Modal-project)

ADD_EXECUTABLE(GC graphCuts.cpp image.cpp image.h maxflow/graph.cpp alphaexpansion.cpp alphaexpansion.h blending.cpp blending.h distance.cpp distance.h labelings.cpp labelings.h openfiles.cpp openfiles.h unitedimages.cpp unitedimages.h)
TARGET_LINK_LIBRARIES(GC ${OpenCV_LIBS})
