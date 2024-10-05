project(FramelessWindow)

set(_core_src
    Src/core/qwkglobal.h
    Src/core/qwkglobal_p.h
    Src/core/qwkglobal.cpp
    Src/core/windowagentbase.h
    Src/core/windowagentbase_p.h
    Src/core/windowagentbase.cpp
    Src/core/windowitemdelegate_p.h
    Src/core/windowitemdelegate.cpp
    Src/core/kernel/nativeeventfilter_p.h
    Src/core/kernel/nativeeventfilter.cpp
    Src/core/kernel/sharedeventfilter_p.h
    Src/core/kernel/sharedeventfilter.cpp
    Src/core/kernel/winidchangeeventfilter_p.h
    Src/core/kernel/winidchangeeventfilter.cpp
    Src/core/shared/systemwindow_p.h
    Src/core/contexts/abstractwindowcontext_p.h
    Src/core/contexts/abstractwindowcontext.cpp
)

set(_links_private)

if(WIN32)
    list(APPEND _core_src
        Src/core/qwindowkit_windows.h
        Src/core/qwindowkit_windows.cpp
        Src/core/shared/qwkwindowsextra_p.h
        Src/core/shared/windows10borderhandler_p.h
    )
elseif(APPLE)
else()
    list(APPEND _core_src
        Src/core/qwindowkit_linux.h
    )
endif()

if(QWINDOWKIT_ENABLE_QT_WINDOW_CONTEXT)
    list(APPEND _core_src
        Src/core/contexts/qtwindowcontext_p.h
        Src/core/contexts/qtwindowcontext.cpp
    )
else()
    if(WIN32)
        list(APPEND _core_src
            Src/core/contexts/win32windowcontext_p.h
            Src/core/contexts/win32windowcontext.cpp
        )
        list(APPEND _links_private uxtheme)
    elseif(APPLE)
        list(APPEND _core_src
            Src/core/contexts/cocoawindowcontext_p.h
            Src/core/contexts/cocoawindowcontext.mm
        )
        list(APPEND _links_private
            "-framework Foundation"
            "-framework Cocoa"
            "-framework AppKit"
        )
    else()
        list(APPEND _core_src
            Src/core/contexts/qtwindowcontext_p.h
            Src/core/contexts/qtwindowcontext.cpp
        )
    endif()
endif()

set(_sync_include_options)

if(QWINDOWKIT_ENABLE_STYLE_AGENT)
    list(APPEND _core_src
        Src/core/style/styleagent.h
        Src/core/style/styleagent_p.h
        Src/core/style/styleagent.cpp
    )

    if(WIN32)
        list(APPEND _core_src Src/core/style/styleagent_win.cpp)
    elseif(APPLE)
        list(APPEND _core_src Src/core/style/styleagent_mac.mm)
    else()
        list(APPEND _core_src Src/core/style/styleagent_linux.cpp)
    endif()
else()
    list(APPEND _sync_include_options EXCLUDE "Src/core/style/\\.+")
endif()

set(_widgets_src
    Src/widgets/qwkwidgetsglobal.h
    Src/widgets/widgetitemdelegate_p.h
    Src/widgets/widgetitemdelegate.cpp
    Src/widgets/widgetwindowagent.h
    Src/widgets/widgetwindowagent_p.h
    Src/widgets/widgetwindowagent.cpp
)

if(WIN32)
    list(APPEND _widgets_src Src/widgets/widgetwindowagent_win.cpp)
elseif(APPLE)
    list(APPEND _widgets_src Src/widgets/widgetwindowagent_mac.cpp)
endif()

set(SRCS 
    Src/windowbar.h
    Src/windowbar.cpp
    Src/windowbar_p.h
    Src/windowbutton.h
    Src/windowbutton.cpp
    Src/windowbutton_p.h
    Include/FramelessWindow/FramelessWindow.h
    Src/FramelessWindow.cpp
)

include_directories(Include)

add_library(${PROJECT_NAME} STATIC ${_core_src} ${_widgets_src} ${SRCS} Resources/windowbar.qrc)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Core Qt::CorePrivate Qt::Gui Qt::GuiPrivate Qt::Widgets Qt::WidgetsPrivate)
