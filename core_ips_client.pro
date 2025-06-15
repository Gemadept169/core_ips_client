QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    api/sot_service.grpc.pb.cc \
    api/sot_service.pb.cc \
    grpc/grpcstub.cpp \
    grpc/sotreader.cpp \
    main.cpp \
    mainwindow.cpp \
    video/cvvideocapture.cpp

HEADERS += \
    api/sot_service.grpc.pb.h \
    api/sot_service.pb.h \
    grpc/grpcstub.h \
    grpc/sotreader.h \
    mainwindow.h \
    video/cvvideocapture.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

##### Libraries including
## Opencv4
INCLUDEPATH += /home/thai/Releases/opencv/include/opencv4
# Include all libs of opencv4
unix:!macx: LIBS += $(shell pkg-config opencv4 --libs)

## Grpc
INCLUDEPATH += /home/thai/Releases/grpc/include
unix:!macx: LIBS += $(shell pkg-config grpc --libs)
