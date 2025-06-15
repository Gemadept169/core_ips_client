#ifndef CVVIDEOCAPTURE_H
#define CVVIDEOCAPTURE_H

#include <QObject>
#include <QTimer>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/core.hpp>

class CvVideoCapture : public QObject
{
    Q_OBJECT
public:
    explicit CvVideoCapture(const QString &rtspPath, const uint &fps = 30, QObject *parent = nullptr);
    CvVideoCapture(const CvVideoCapture &) = delete;
    CvVideoCapture &operator=(const CvVideoCapture &) = delete;
    ~CvVideoCapture();

signals:
    void hasVideoNewFrame(const cv::Mat &frame);

public slots:
    void startCapture();
    void stopCapture();

private slots:
    void readFrame();

private:
    cv::Ptr<cv::VideoCapture> _cap;
    uint _fps;
    QString _rtspPath;
    QTimer *_readTimer;
};

#endif // CVVIDEOCAPTURE_H
