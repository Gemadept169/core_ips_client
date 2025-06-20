#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMetaObject>
#include "video/cvvideocapture.h"
#include "grpc/grpcclient.h"
#include "video/cvvideocapture.h"
#include <QThread>
#include <QMouseEvent>

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

enum class TrackState : unsigned char {
    STOP = 0,
    PAUSE,
    START
};

signals:
    void startTracking(const BBox& initBox);
    void stopTracking();

public slots:
    void handleVideoNewFrame(const cv::Mat& frame);
    void handleNewSotInfo(const SotInfo& info);

private slots:
    void on_btnTest_clicked();

    void on_btnSot_toggled(bool checked);

private:
    void scrollMouseOnStream(float deltaY);
    bool eventFilter(QObject *target, QEvent *event) override;
    void drawTrackBox(cv::Mat &frame);

    Ui::MainWindow *ui;
    int _initBoxSize;
    int _trackLostCounter;
    SotInfo _sotInfo;
    bool _hasNewSotInfo;
    TrackState _trackState;
    CvVideoCapture *_cap;
    QThread _capThread;
    GrpcClient *_grpcClient;
    QThread _grpcThread;
};
#endif // MAINWINDOW_H
