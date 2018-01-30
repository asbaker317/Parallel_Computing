cmake_minimum_required(VERSION 3.8)
project(MST_CC)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        create_edgelist.cpp
        create_tree.hpp
        Insert_Delete_ConC.hpp
        Insert_Delete_MST.hpp
        Instructions
        kurskal_MST.hpp
        main_code1x.cpp
        main_code1x.o
        Makefile
        modifyWT.hpp
        readin_data.hpp
        rewire.hpp
        SP_structure.hpp
        traversalEx.cpp
        tree_info.hpp boruvka.cpp btest.cpp Bor_FAL.cpp append.cpp)

add_executable(MST_CC ${SOURCE_FILES})