QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

## Grpc++
INCLUDEPATH += /home/thai/Releases/grpc/include
unix:!macx: LIBS += -L/home/thai/Releases/grpc/lib \
-lgrpc++ -lgrpc -lgpr -lprotobuf  -labsl_log_internal_check_op -labsl_log_internal_message \
-labsl_log_internal_nullguard -labsl_synchronization -labsl_cord -labsl_cordz_info -labsl_cordz_functions

##### Libraries including
## Opencv4
INCLUDEPATH += /home/thai/Releases/opencv/include/opencv4
# Include all libs of opencv4
unix:!macx: LIBS += -L/home/thai/Releases/opencv/lib \
-lopencv_imgproc -lopencv_core -lopencv_videoio -lopencv_highgui \
-lopencv_imgcodecs -lopencv_freetype -lopencv_ximgproc

CONFIG += c++17

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

