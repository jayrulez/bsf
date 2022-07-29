function(add_sample SAMPLE_NAME)
    if (WIN32)
        file(GLOB SOURCE_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_NAME}/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_NAME}/*.cpp"
        )
        add_executable(${SAMPLE_NAME} WIN32 ${SOURCE_FILES})
        target_link_libraries(${SAMPLE_NAME} bsf)
        target_include_directories(${SAMPLE_NAME} PRIVATE ${BSF_SOURCE_DIR})
        
        install(
            TARGETS ${SAMPLE_NAME}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
        set_property(TARGET ${SAMPLE_NAME} PROPERTY FOLDER "Samples")
    endif()
endfunction()

add_sample(01_HelloWorld)