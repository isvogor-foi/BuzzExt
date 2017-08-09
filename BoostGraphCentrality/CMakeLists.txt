cmake_minimum_required(VERSION 2.6)
Project(BoostGraphCentrality)
 
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.58 COMPONENTS graph)

if(Boost_FOUND)
  MESSAGE( STATUS "Include -> : " ${Boost_INCLUDE_DIRS} )
  MESSAGE( STATUS "Lib -> : " ${Boost_LIBRARIES} )
endif()

SET(HEADERS GraphOperations.h zip.h)

# build SO 
ADD_LIBRARY(graph_operations SHARED GraphOperations.cpp ${HEADERS})
target_link_libraries(graph_operations ${Boost_LIBRARIES})

ADD_EXECUTABLE(forest_call forest_call ${HEADERS})
target_link_libraries(forest_call graph_operations ${Boost_LIBRARIES})

ADD_EXECUTABLE(tree_call tree_call ${HEADERS})
target_link_libraries(tree_call graph_operations ${Boost_LIBRARIES})

