# ================WINDOWS==================
# mkdir build; cd build;
# cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 11" ..
# msbuild List_vs_Vector.sln /p:Configuration=Release
# 
# and execute the example at: Release\list_vs_vector.exe
#
# IMPORTANT: If you do not do the steps above you might 
#            get debug and not release version. The performance
#            difference between them are HUGE!



# ================LINUX==================
# mkdir build; cd build;
# cmake; make
# 
# IMPORTANT: If the performance difference between linked-list and vector is small
#            or if the performance result is not at all what you expected then please
#            verify that you are using RELEASE and not DEBUG version
#            I.e. call "make" with "make VERBOSE=1"
#                 this will show what make settings you have. 
#                 03 should be used for this performance test
cmake_minimum_required (VERSION 2.8)
ENABLE_LANGUAGE(CXX)
set(CMAKE_BUILD_TYPE Release)
project (List_vs_Vector) 

IF(UNIX)
       MESSAGE("")
       MESSAGE("cmake for *NIX ")
       MESSAGE("if cmake finishes OK, do make")
       MESSAGE("then run ./list_vs_vector")
       MESSAGE("or run ./list_vs_vector_POD")
       MESSAGE("")
       set(CMAKE_CXX_FLAGS "-Wall -Wunused -std=c++0x")
ENDIF(UNIX)

IF(WIN32)   	
       MESSAGE("")
       MESSAGE("")
       MESSAGE("cmake for Visual Studio 2011")
       MESSAGE("if cmake finishes OK, do 'msbuild List_vs_Vector.sln /p:Configuration=Release'")
       MESSAGE("then run 'Release\\list_vs_vector.exe'")
       MESSAGE("or run 'Release\\list_vs_vector_POD.exe'")
ENDIF(WIN32)

# =================
# Generic steps 
# =================
  include_directories(../src)
  # create the test executable
  add_executable(list_vs_vector ../src/main.cpp  ../src/g2_chrono.h ../src/linear_performance.h)

add_executable(list_vs_vector_POD ../src/main_POD_comparison.cpp)

target_link_libraries(list_vs_vector ${PLATFORM_LINK_LIBRIES})
target_link_libraries(list_vs_vector_POD ${PLATFORM_LINK_LIBRIES})




