#NameSpace with CMake utility functions
#- include it after project(...)

################################################################ Compile Definitions & Options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(/W4 /MP)                        #warning level, multi-process compilation
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall)                          #warning level
endif()

################################################################ INSTALL
#usage:   Ns_install(${PROJECT_NAME} ${CMAKE_SOURCE_DIR})
function(Ns_install target path)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "${path}")
    endif()
    if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME) #root or built alone
        message(CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX})
    endif()

    install(TARGETS ${target}
        ARCHIVE         DESTINATION ${CMAKE_INSTALL_PREFIX}
        LIBRARY         DESTINATION ${CMAKE_INSTALL_PREFIX}
        OBJECTS         DESTINATION ${CMAKE_INSTALL_PREFIX} #doesn't work as expected!!! .OBJ files will be in something like: objects-Debug/X/X.obj
        RUNTIME         DESTINATION ${CMAKE_INSTALL_PREFIX}
        PUBLIC_HEADER   DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
endfunction()

################################################################ VisualStudio
function(Ns_VisualStudioSpecific target cmd arg)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${Src} ${Hdr} ${HdrPrv})

    file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} nativePath)
    if(TARGET ${target})
        set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
        set_target_properties(${target} PROPERTIES
            VS_STARTUP_PROJECT              ${PROJECT_NAME}
            VS_DEBUGGER_COMMAND             "${cmd}"
            VS_DEBUGGER_COMMAND_ARGUMENTS   "${arg}"
            VS_DEBUGGER_WORKING_DIRECTORY   "${nativePath}"
    )
    endif()
    set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1 PARENT_SCOPE)
endfunction()