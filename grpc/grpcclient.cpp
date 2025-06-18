#include "grpcclient.h"
#include "sotclient.h"

GrpcClient::GrpcClient(QObject* parent)
    : QObject(parent),
    _sotClient(nullptr) {

}

void GrpcClient::atStarted() {
    _sotClient = new SotClient(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()), this);
}

GrpcClient::~GrpcClient() {

}
