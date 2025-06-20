#include "grpcclient.h"
#include "sotclient.h"
#include <QDebug>
#include <QThread>

GrpcClient::GrpcClient(QObject* parent)
    : QObject(parent),
    _sotClient(nullptr) {

}

void GrpcClient::atStarted() {
    qDebug() << "GrpcClient atStarted:" << QThread::currentThreadId();
    QThread* grpcThread = QThread::create([this]() -> void {
        this->_sotClient = new SotClient(grpc::CreateChannel("localhost:52124", grpc::InsecureChannelCredentials()), this);
    });
    QObject::connect(grpcThread, &QThread::finished, grpcThread, &QThread::deleteLater);
    grpcThread->start();
}

void GrpcClient::startTracking(const BBox &box) {
    _sotClient->track(box);
}

void GrpcClient::stopTracking() {
    qDebug() << "[GrpcClient::stopTracking]";
    _sotClient->stop();
}

GrpcClient::~GrpcClient() {
    if (_sotClient) {
        delete _sotClient;
    }
}
