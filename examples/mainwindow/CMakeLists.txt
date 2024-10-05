project(FramelessWindowDemo)

file(GLOB _src *.h *.cpp)

add_executable(${PROJECT_NAME} ${_src} mainwindow.qrc)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Widgets FramelessWindow)