#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMetaObject>
#include "video/cvvideocapture.h"
#include "grpc/grpcclient.h"
#include "video/cvvideocapture.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handleVideoNewFrame(const cv::Mat& frame);

private slots:
    void on_pushButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    CvVideoCapture *_cap;
    QThread _capThread;
    GrpcClient *_grpcClient;
    QThread _grpcThread;
};
#endif // MAINWINDOW_H
