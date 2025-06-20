#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QDebug>
#include <opencv2/imgproc.hpp>
#include "common.h"
#include <cmath>


namespace {
    QImage convertMat2QImage(const cv::Mat &inMat) {
        switch (inMat.type()) {
        // 8-bit, 4 channel
        case CV_8UC4: {
            QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_ARGB32);

            return image;
        }

            // 8-bit, 3 channel
        case CV_8UC3: {
            QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888);

            return image.rgbSwapped();
        }

            // 8-bit, 1 channel
        case CV_8UC1: {
            static QVector<QRgb> sColorTable;

            // only create our color table once
            if (sColorTable.isEmpty()) {
                for (int i = 0; i < 256; ++i)
                    sColorTable.push_back(qRgb(i, i, i));
            }

            QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8);

            image.setColorTable(sColorTable);

            return image;
        }

        default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
        }

        return QImage();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    _initBoxSize(50),
    _trackLostCounter(0),
    _hasNewSotInfo(false),
    _trackState(TrackState::STOP),
    _cap(new CvVideoCapture("rtsp://localhost:8555/cam")),
    _grpcClient(new GrpcClient())
{
    // Setup UI
    ui->setupUi(this);
    ui->lblVideo->installEventFilter(this);
    // Register meta types
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<BBox>("BBox");
    qRegisterMetaType<SotInfo>("SotInfo");

    _cap->moveToThread(&_capThread);
    QObject::connect(_cap, &CvVideoCapture::hasVideoNewFrame, this, &MainWindow::handleVideoNewFrame);
    QObject::connect(&_capThread, &QThread::started, _cap, &CvVideoCapture::startCapture);
    QObject::connect(&_capThread, &QThread::finished, _cap, &CvVideoCapture::deleteLater);

    _grpcClient->moveToThread(&_grpcThread);
    QObject::connect(&_grpcThread, &QThread::started, _grpcClient, &GrpcClient::atStarted);
    QObject::connect(&_grpcThread, &QThread::finished, _grpcClient, &GrpcClient::deleteLater);
    QObject::connect(this, &MainWindow::startTracking, _grpcClient, &GrpcClient::startTracking);
    QObject::connect(this, &MainWindow::stopTracking, _grpcClient, &GrpcClient::stopTracking);
    QObject::connect(_grpcClient, &GrpcClient::hasSotTrackNewResponse, this, &MainWindow::handleNewSotInfo);

    _grpcThread.start();
    _capThread.start();

    qDebug() << "MainWindow ctor:" << QThread::currentThreadId();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleVideoNewFrame(const cv::Mat &frame) {
    cv::Mat displayFrame = frame;

    if (_trackState == TrackState::PAUSE) {
        int centroidX = static_cast<int>(1920.0f / 2);
        int centroidY = static_cast<int>(1080.0f / 2);
        cv::Point2i curCenterPoint(centroidX, centroidY);
        cv::rectangle(displayFrame,
            cv::Rect(curCenterPoint.x - static_cast<int>(_initBoxSize / 2),
                curCenterPoint.y - static_cast<int>(_initBoxSize / 2),
                _initBoxSize, _initBoxSize),
            cv::Scalar(21, 229, 218), 2);
    } else if (_trackState == TrackState::START && _hasNewSotInfo) {
        drawTrackBox(displayFrame);
        _hasNewSotInfo = false;
    }

    int width = ui->lblVideo->width();
    int height = ui->lblVideo->height();
    cv::resize(displayFrame, displayFrame, cv::Size(width, height));
    QImage image = convertMat2QImage(displayFrame);
    ui->lblVideo->setPixmap(QPixmap::fromImage(image));
    ui->lblVideo->adjustSize();
}

void MainWindow::handleNewSotInfo(const SotInfo &info) {
    _hasNewSotInfo = true;
    _sotInfo = info;
}

void MainWindow::scrollMouseOnStream(float deltaY) {
    const float wheelDeltaY = deltaY / (120.0f * 20.0f);
    _initBoxSize = static_cast<int>(_initBoxSize * (1.0f - wheelDeltaY));
    _initBoxSize = std::fmin(std::fmax(_initBoxSize, 35), 150);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    if (target == ui->lblVideo) {
        switch (event->type()) {
        case QMouseEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            // Tracking mode
            constexpr float wRatio = 1920.0f * 1.0f / 1366.0f;
            constexpr float hRatio = 1080.0f * 1.0f / 768.0f;
            if (mouseEvent->button() == Qt::LeftButton && _trackState == TrackState::PAUSE) {
                QPoint pos = mouseEvent->pos();
                const int x = static_cast<int>(pos.x() * wRatio);
                const int y = static_cast<int>(pos.y() * hRatio);
                _trackState = TrackState::START;
                qDebug() << "MainWindow before startTracking:" << QThread::currentThreadId();
                emit startTracking(BBox(x, y, _initBoxSize, _initBoxSize));
            } else {
                emit stopTracking();
            }
            return true;
        }
        case QEvent::Wheel: {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
            scrollMouseOnStream(wheelEvent->angleDelta().y());
            return true;
        }
        default:
            return false;
        }
    }
    return false;
}

void MainWindow::drawTrackBox(cv::Mat &frame) {
    cv::Rect rectObj(static_cast<int>(_sotInfo.bbox.xtl),
                     static_cast<int>(_sotInfo.bbox.ytl),
                     static_cast<int>(_sotInfo.bbox.width),
                     static_cast<int>(_sotInfo.bbox.height));
    if (_sotInfo.score > 0.9f) {
        cv::rectangle(frame, rectObj, cv::Scalar(0, 255, 0), 2); // Green if ok
        if (_trackLostCounter != 0) {
            _trackLostCounter = 0;
        }
    } else {
        _trackLostCounter += 1;
        cv::rectangle(frame, rectObj, cv::Scalar(0, 0, 255), 2); // Red if lost
    }
    if (_trackLostCounter > 15) {
        _trackLostCounter = 0;
        emit stopTracking();
    }
}


void MainWindow::on_btnTest_clicked() {
    emit stopTracking();
}


void MainWindow::on_btnSot_toggled(bool checked) {
    _trackLostCounter = 0;
    if (checked) {
        _trackState = TrackState::PAUSE;
    } else {
        _trackState = TrackState::STOP;
    }
}

