include(openglwindow.pri)

SOURCES += \
    main.cpp \
    GameWindows.cpp
target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target

HEADERS += \
    GameWindows.h
