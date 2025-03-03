cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

project(ExamplePlugin
    VERSION 1.0.0.0
    DESCRIPTION "ExamplePlugin VST 3 Plug-in"
)

add_subdirectory(external)

smtg_enable_vst3_sdk()

smtg_add_vst3plugin(ExamplePlugin
    source/version.h
    source/exampleplugincids.h
    source/examplepluginprocessor.h
    source/examplepluginprocessor.cpp
    source/exampleplugincontroller.h
    source/exampleplugincontroller.cpp
    source/examplepluginentry.cpp
    resource/exampleplugineditor.uidesc
)

target_link_libraries(ExamplePlugin
    PRIVATE
        sdk
        vstgui_support
)

smtg_target_add_plugin_resources(ExamplePlugin
    RESOURCES
        resource/exampleplugineditor.uidesc
        resource/background.png
        resource/animation_knob.png
        resource/onoff_button.png
        resource/background_2x.png
        resource/animation_knob_2x.png
        resource/onoff_button_2x.png
        resource/background_3x.png
        resource/animation_knob_3x.png
        resource/onoff_button_3x.png
)

smtg_target_add_plugin_snapshots (ExamplePlugin
    RESOURCES
        resource/32C50013FF5F5CB4871C312DB4F42368_snapshot.png
        resource/32C50013FF5F5CB4871C312DB4F42368_snapshot_2.0x.png
)

smtg_target_configure_version_file(ExamplePlugin)

if(SMTG_MAC)
    smtg_target_set_bundle(ExamplePlugin
        BUNDLE_IDENTIFIER net.steinberg.example-plugin
        COMPANY_NAME "Steinberg Media Technologies GmbH"
    )
    smtg_target_set_debug_executable(ExamplePlugin
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(ExamplePlugin PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ExamplePlugin)

        smtg_target_set_debug_executable(ExamplePlugin
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
