
set(CEF_TARGET ${PROJECT_NAME})
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
SET_CEF_TARGET_OUT_DIR()
if(OS_LINUX)
    # Executable target.
    add_executable(${CEF_TARGET} headless.cpp)
    SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
    add_dependencies(${CEF_TARGET} libcef_dll_wrapper)

    add_library(epd ../tmp/epd4in2.cpp ../tmp/epdif.cpp ../tmp/epdpaint.cpp ../tmp/epd4in2.h ../tmp/epdif.h ../tmp/epdpaint.h
            ../tmp/font8.cpp ../tmp/font12.cpp ../tmp/font16.cpp ../tmp/font20.cpp ../tmp/font24.cpp ../tmp/fonts.h)
    add_library(dither dither.cpp ../inc/dither.h)
    include_directories(../inc ../src ../tmp)

    target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper epd ${CEF_STANDARD_LIBS} ${OpenCV_LIBS} ${PYTHON_LIBRARIES} wiringPi)

    # Set rpath so that libraries can be placed next to the executable.
    set_target_properties(${CEF_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
    set_target_properties(${CEF_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
    set_target_properties(${CEF_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})

    # Copy binary and resource files to the target output directory.
    COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")

    COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
    if (EXISTS "${CEF_BINARY_DIR}/libminigbm.so")
        COPY_FILES("${CEF_TARGET}" "libminigbm.so" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
    endif()

    # Set SUID permissions on the chrome-sandbox target.
    SET_LINUX_SUID_PERMISSIONS("${CEF_TARGET}" "${CEF_TARGET_OUT_DIR}/chrome-sandbox")

    # Install Script
    install(TARGETS ${CEF_TARGET}
            RUNTIME DESTINATION bin)
    install(DIRECTORY "${CEF_RESOURCE_DIR}/"
            DESTINATION bin)
    install(DIRECTORY "${CEF_BINARY_DIR}/"
            DESTINATION bin)
endif()