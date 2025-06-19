#include "grpcclient.h"
#include "sotclient.h"

GrpcClient::GrpcClient(QObject* parent)
    : QObject(parent),
    _sotClient(nullptr) {

}

void GrpcClient::atStarted() {
    this->_sotClient = new SotClient(grpc::CreateChannel("localhost:52124", grpc::InsecureChannelCredentials()), this);
}

void GrpcClient::startTracking(const BBox &box) {
    _sotClient->track(box);
}

void GrpcClient::stopTracking() {
    _sotClient->stop();
}

GrpcClient::~GrpcClient() {
    if (_sotClient) {
        delete _sotClient;
    }
}
