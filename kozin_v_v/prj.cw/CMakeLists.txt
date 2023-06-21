cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_TOOLCHAIN_FILE "path to toolchain file")

project(maskEditor)




find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Stb REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(glad REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)



add_executable(maskEditor main.cpp
        program.cpp
        program.h
        shader.cpp
        shader.h
        imgui_extension.cpp
        imgui_extension.h
        image.cpp
        image.h
        images.cpp
        images.h
        layer.cpp
        layer.h
        glarraybuffer.cpp
        glarraybuffer.h
        glprogram.cpp
        glprogram.h
        yolo_seg.cpp
        yolo_seg.h
        yolov5_seg_utils.cpp
        yolov5_seg_utils.h
        )



target_link_libraries(maskEditor
        ${OpenCV_LIBS}
        ${OPENGL_LIBRARIES}
        ${GLM_LIBRARIES}
        glad::glad
        imgui::imgui
        unofficial::nativefiledialog::nfd

        )



option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
    add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)





configure_file(include/Roboto-Medium.ttf include/Roboto-Medium.ttf COPYONLY)
configure_file(include/fontawesome-webfont.ttf include/fontawesome-webfont.ttf COPYONLY)



file(GLOB ONNX_FILES "include/*.onnx")
if(ONNX_FILES)
list(GET ONNX_FILES 0 FIRST_ONNX_FILE)
get_filename_component(ONNX_FILE_NAME ${FIRST_ONNX_FILE} NAME)
configure_file("${FIRST_ONNX_FILE}" "include/${ONNX_FILE_NAME}" COPYONLY)
else()
message(FATAL_ERROR "File .onnx not found in include/")
endif()



install(TARGETS maskEditor DESTINATION bin)

install(TARGETS maskEditor
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DESTINATION bin
        )


install(FILES
        include/Roboto-Medium.ttf
        include/fontawesome-webfont.ttf
        include/${ONNX_FILE_NAME}
        DESTINATION bin/include)

install(DIRECTORY
        include/test_images
        DESTINATION bin)