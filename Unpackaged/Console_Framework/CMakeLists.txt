#----------------------------------------------------------------------------------------------------------------------
#
#----------------------------------------------------------------------------------------------------------------------
project(Unpackaged_Console_Framework LANGUAGES CXX)

add_executable(Unpackaged_Console_Framework
    app.manifest
    main.cpp
)

target_precompile_headers(Unpackaged_Console_Framework
    PRIVATE
        pch.h
)

# By linking to the '_Framework' variant of the Windows App SDK, the application will take a dependency on the Framework
# package for the Windows App SDK runtime.
#
target_link_libraries(Unpackaged_Console_Framework
    PRIVATE
        Microsoft.WindowsAppSDK.Foundation_Framework
        Microsoft.Windows.ImplementationLibrary
)

# Setting 'WindowsAppSdk*' target properties on _consumers_ of 'Microsoft.WindowsAppSDK.Foundation_Framework' controls
# the inclusion of the Windows App SDK Bootstrap infrastructure.
set_target_properties(Unpackaged_Console_Framework
    PROPERTIES
        # Controls whether the 'dynamic dependency' bootstrap will be added to the current target
        WindowsAppSdkBootstrapInitialize TRUE

        # Controls whether the 'deployment manager' bootstrap will be added to the current target
        WindowsAppSdkDeploymentManagerInitialize FALSE
)

post_build_runtime_dll_copy(Unpackaged_Console_Framework)
