#----------------------------------------------------------------------------------------------------------------------
#
#----------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.31)

cmake_policy(SET CMP0141 NEW) # MSVC debug information format flags are selected by an abstraction.
cmake_policy(SET CMP0117 NEW) # MSVC RTTI flag /GR is not added to CMAKE_CXX_FLAGS by default.

if(CMAKE_GENERATOR MATCHES "^Visual Studio")
    # To prevent builds (including try_compile) from picking up top-level 'Directory.Build.*' files,
    # write versions specific to the CMake builds at the root of the CMake build.
    file(WRITE "${CMAKE_BINARY_DIR}/Directory.Build.rsp" "-m -graphBuild:true -p:UseMultiToolTask=true -nodeReuse:false -terminalLogger:auto")
    file(WRITE "${CMAKE_BINARY_DIR}/Directory.Build.props" "<Project></Project>")
    file(WRITE "${CMAKE_BINARY_DIR}/Directory.Build.targets" "<Project></Project>")
elseif(CMAKE_GENERATOR MATCHES "^Ninja")
    if(NOT (DEFINED ENV{Platform}))
        message(FATAL_ERROR "When using the Ninja generator, you must build from a platform-specific Developer Command Prompt.")
    endif()
endif()

project(NuGetCMakePackage_Samples LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    NuGetCMakePackage
    GIT_REPOSITORY https://github.com/mschofie/NuGetCMakePackage
    GIT_TAG 73b12f864e742825ae2d1f58fa2064000d9a7187
)

FetchContent_MakeAvailable(NuGetCMakePackage)

add_nuget_packages(
    PACKAGES
        Microsoft.Windows.CppWinRT 2.0.*
        Microsoft.Windows.ImplementationLibrary 1.0.*
        Microsoft.WindowsAppSDK.Foundation 1.8.*
        Microsoft.WindowsAppSDK.ML 1.8.*

        # The 'Microsoft.WindowsAppSDK.Runtime' isn't used directly, other 'Microsoft.WindowsAppSDK' packages will check for its presence to
        # surface dependent functionality.
        Microsoft.WindowsAppSDK.Runtime 1.8.*
    CONFIG_FILE
        "${CMAKE_SOURCE_DIR}/nuget.config"
    LOCK_FILE
        "${CMAKE_SOURCE_DIR}/packages.lock.json"
)

find_package(Microsoft.Windows.ImplementationLibrary CONFIG REQUIRED)
find_package(Microsoft.WindowsAppSDK.Foundation CONFIG REQUIRED)

macro(post_build_runtime_dll_copy target_name)
    if(WIN32)
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND "${CMAKE_COMMAND};-E;$<IF:$<BOOL:$<TARGET_RUNTIME_DLLS:${target_name}>>,copy;$<TARGET_RUNTIME_DLLS:${target_name}>;$<TARGET_FILE_DIR:${target_name}>,true>"
            COMMAND_EXPAND_LISTS
        )
    endif()
endmacro()

# Enable CMake support for Generators to include folders in their generated build output.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY APPEND PROPERTY VS_SOLUTION_ITEMS
    .clangd
    .editorconfig
    .gitignore
    CMakePresets.json
    LICENSE
    NOTICE
    nuget.config
    packages.lock.json
    README.md
    SECURITY.md
)

# Common configuration
#
include(./Configuration.cmake)
link_libraries(Configuration)

# Subdirectories
#
add_subdirectory(Unpackaged)
