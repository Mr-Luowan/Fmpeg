cmake_minimum_required(VERSION 3.10.2)

project("fmpeg")


# 1. 定义so库和头文件所在目录，方面后面使用
set(ffmpeg_lib_dir ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
message(WARNING "库文件路径 lib file path --> ${ffmpeg_lib_dir}")
set(ffmpeg_head_dir ${CMAKE_SOURCE_DIR}/ffmpeginclude)

# 2. 添加头文件目录
include_directories(
        ${ffmpeg_head_dir}
        ${CMAKE_SOURCE_DIR}/player
        ${CMAKE_SOURCE_DIR}/player/decoder
        ${CMAKE_SOURCE_DIR}/player/render/video
        ${CMAKE_SOURCE_DIR}/opensl
        ${CMAKE_SOURCE_DIR}/gles
        ${CMAKE_SOURCE_DIR}/utils
        ${CMAKE_SOURCE_DIR}/glm
        ${CMAKE_SOURCE_DIR}/stbi_img
)


# 3. 添加ffmpeg相关的so库
add_library( avutil
        SHARED
        IMPORTED )
set_target_properties( avutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavutil.so )

add_library( swresample
        SHARED
        IMPORTED )
set_target_properties( swresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswresample.so )

add_library( avcodec
        SHARED
        IMPORTED )
set_target_properties( avcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavcodec.so )

add_library( avfilter
        SHARED
        IMPORTED)
set_target_properties( avfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavfilter.so )

add_library( swscale
        SHARED
        IMPORTED)
set_target_properties( swscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswscale.so )

add_library( avformat
        SHARED
        IMPORTED)
set_target_properties( avformat
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavformat.so )

add_library( avdevice
        SHARED
        IMPORTED)
set_target_properties( avdevice
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavdevice.so )

add_library( h264
        SHARED
        IMPORTED)
set_target_properties( h264
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libh264.so )

file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/player/*.cpp
        ${CMAKE_SOURCE_DIR}/player/decoder/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/video/NativeRender.cpp
        ${CMAKE_SOURCE_DIR}/opensl/*.cpp
        ${CMAKE_SOURCE_DIR}/gles/*.cpp
        ${CMAKE_SOURCE_DIR}/utils/*.cpp
        ${CMAKE_SOURCE_DIR}/stbi_img/*.cpp
        native-lib.cpp
        )

add_library(
        fmpeg

        SHARED

        ${src-files})

# 查找代码中使用到的系统库
find_library(
        log-lib
        log)



set(third_libs
        avutil
        swresample
        avcodec
        avfilter
        swscale
        avformat
        avdevice
        h264)


set(native-libs
        android
        mediandk
        EGL
        GLESv3
        OpenSLES
        )


target_link_libraries(
        fmpeg
        # 4. 连接 FFmpeg 相关的库
        ${third_libs}
        ${native-libs}
        ${log-lib})