#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QDebug>
#include <opencv2/imgproc.hpp>
#include "common.h"

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
    _cap(new CvVideoCapture("rtsp://localhost:8555/cam")),
    _grpcClient(new GrpcClient())
{
    ui->setupUi(this);

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


    _grpcThread.start();
    _capThread.start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleVideoNewFrame(const cv::Mat &frame) {
    int width = ui->lblVideo->width();
    int height = ui->lblVideo->height();
    cv::Mat displayFrame = frame;
    cv::resize(displayFrame, displayFrame, cv::Size(width, height));
    QImage image = convertMat2QImage(displayFrame);
    ui->lblVideo->setPixmap(QPixmap::fromImage(image));
    ui->lblVideo->adjustSize();
}

void MainWindow::on_pushButton_toggled(bool checked)
{

}
