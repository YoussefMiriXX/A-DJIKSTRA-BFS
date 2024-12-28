QT = core
QT += gui
QT += widgets

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../launch.cpp \
       ../vertex.cpp\
        ../edge.cpp\
        ../graph.cpp\
        ../QVertex.cpp\
        ../QGraph.cpp\
        ../QEdge.cpp\
        ../commify.cpp\
        ../mainwindow.cpp\

HEADERS += \
        ../vertex.h\
        ../edge.h\
        ../graph.h\
        ../QVertex.h\
        ../QGraph.h\
        ../QEdge.h\
        ../commify.h\
        ../mainwindow.h\


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
