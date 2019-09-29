cmake_minimum_required(VERSION 3.4.1)
add_library( compress-jpeg SHARED cpp/compress-jpeg.cpp)

add_library(turbojpeg STATIC IMPORTED)
set_target_properties(turbojpeg PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/jniLibs/armeabi-v7a/libturbojpeg.a)

include_directories(${CMAKE_SOURCE_DIR}/cpp/include)





find_library( log-lib
              log )

target_link_libraries(  compress-jpeg
                        turbojpeg
                        #jnigraphics是安卓NDK目录中直接有的
                        jnigraphics
                        ${log-lib} )