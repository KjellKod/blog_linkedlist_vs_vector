#Ideone.com examples to be compiled and run
# on your computer
#===================
#ideone_62Emz.cpp : Linear insert of random elements in sorted order
#                   [http://ideone.com/62Emz]
#
#
#ideone_tLUeK.cpp: Sort comparison vector vs list [http://ideone.com/tLUeK]
#
#
#ideone_u5wbd.java: Similar to the linear insertion of random elements in 
#                   order but with JAVA code. [http://ideone.com/u5wbd]
#          The java example is not compiled here but can easily be done manually
#          javac ideone_u5wbd.java
#          java ideone_u5wb
#
#ideone_XprUU.cpp: "Smart" linked-list. Saving of last insertion position pointer
#                  to possibly avoid traversing unnecessarily long distance.
#                  [ideone.com/XprUU]
#
#
#ideone_W9vpT.cpp: Linear insert of random POD elements. The test is done with
#                  varying POD sizes. On [ideone.com/W9vpT] the test will
#                  timeout before finishing for the largest POD size (256 bytes).
#
#
# ================WINDOWS==================
# mkdir build; cd build;
# cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 11" ..
# msbuild ideone_examples.sln /p:Configuration=Release
# 
# and execute the example at: Release\ideone_xxxx.exe
#
# IMPORTANT: If you do not do the steps above you might 
#            get debug and not release version. The performance
#            difference between them are HUGE!
#
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
project (ideone_examples) 

IF(UNIX)
       MESSAGE("")
       MESSAGE("cmake for *NIX ")
       MESSAGE("if cmake finishes OK, do make")
       MESSAGE("then run the ./ideone_* examples")
       MESSAGE("")
       set(CMAKE_CXX_FLAGS "-Wall -Wunused -std=c++0x")
ENDIF(UNIX)

IF(WIN32)   	
       MESSAGE("")
       MESSAGE("")
       MESSAGE("cmake for Visual Studio 2011")
       MESSAGE("if cmake finishes OK, do 'msbuild ideone_examples.sln /p:Configuration=Release'")
      MESSAGE("then run the ./ideone_* examples")
 ENDIF(WIN32)

# =================
# Generic steps 
# =================
add_executable(ideone_62Emz ../src/ideone_62Emz.cpp) 
add_executable(ideone_tLUeK ../src/ideone_tLUeK.cpp)
add_executable(ideone_W9vpT ../src/ideone_W9vpT.cpp)
add_executable(ideone_XprUU ../src/ideone_XprUU.cpp)
#
# Java has to be run manually
# javac ideone_u5wbd.java
# java ideone_u5wbd

target_link_libraries(ideone_62Emz ${PLATFORM_LINK_LIBRIES})
target_link_libraries(ideone_62Emz ${PLATFORM_LINK_LIBRIES})
target_link_libraries(ideone_W9vpT ${PLATFORM_LINK_LIBRIES})
target_link_libraries(ideone_W9vpT ${PLATFORM_LINK_LIBRIES})




