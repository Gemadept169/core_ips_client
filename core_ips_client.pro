QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

## Grpc++
INCLUDEPATH += /home/capybara/Downloads/grpc-release/release/include
unix:!macx: LIBS += -L/home/capybara/Downloads/grpc-release/release/lib \
-lgrpc++ -lgrpc -lgpr -lprotobuf  -labsl_log_internal_check_op -labsl_log_internal_message \
-labsl_log_internal_nullguard -labsl_synchronization -labsl_cord -labsl_cordz_info -labsl_cordz_functions

##### Libraries including
## Opencv4
INCLUDEPATH += /usr/local/include/opencv4
# Include all libs of opencv4
unix:!macx: LIBS += $(shell pkg-config opencv4 --libs)


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    api/sot_service.grpc.pb.cc \
    api/sot_service.pb.cc \
    grpc/grpcclient.cpp \
    grpc/sotclient.cpp \
    main.cpp \
    mainwindow.cpp \
    video/cvvideocapture.cpp

HEADERS += \
    api/sot_service.grpc.pb.h \
    api/sot_service.pb.h \
    common.h \
    grpc/grpcclient.h \
    grpc/sotclient.h \
    mainwindow.h \
    video/cvvideocapture.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

