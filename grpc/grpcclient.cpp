#include "grpcclient.h"
#include <grpcpp/create_channel.h>
#include <QDebug>
#include <QThread>
#include <QMetaObject>

class GrpcSotTrackImpl : public grpc::ClientReadReactor<core_ips::sot::TrackResponse> {
public:
    GrpcSotTrackImpl(GrpcClient* client, const BBox& box)
        : _client(client) {
        _request.mutable_init_bbox()->set_xtl(box.xtl);
        _request.mutable_init_bbox()->set_ytl(box.ytl);
        _request.mutable_init_bbox()->set_width(box.width);
        _request.mutable_init_bbox()->set_height(box.height);
        _stub = core_ips::sot::Sot::NewStub(_client->_channel);
        _stub->async()->Track(&_context, &_request, this);
        StartRead(&_response);
        StartCall();
    };
    ~GrpcSotTrackImpl() {}
    void OnReadDone(bool ok) override {
        if (ok && _client) {
            SotInfo incomingInfo;
            incomingInfo.bbox.xtl = _response.result().bbox().xtl();
            incomingInfo.bbox.ytl = _response.result().bbox().ytl();
            incomingInfo.bbox.width = _response.result().bbox().width();
            incomingInfo.bbox.height = _response.result().bbox().height();
            incomingInfo.score = _response.result().score();
            emit _client->hasSotTrackNewResponse(incomingInfo);
            StartRead(&_response);
        }
    }
    void OnDone(const grpc::Status& s) override {
        emit _client->hasSotTrackStatus(s);
        delete this;
    }
private:
    std::unique_ptr<core_ips::sot::Sot::Stub> _stub;
    grpc::ClientContext _context;
    core_ips::sot::TrackRequest _request;
    core_ips::sot::TrackResponse _response;
    GrpcClient* _client;
};

class GrpcSotTrackStopImpl : public grpc::ClientUnaryReactor {
public:
    GrpcSotTrackStopImpl(GrpcClient* client)
        : _client(client) {
        _stub = core_ips::sot::Sot::NewStub(_client->_channel);
        _stub->async()->TrackStop(&_context, &_request, &_response, this);
        StartCall();
    };
    void OnDone(const grpc::Status& s) override {
        emit _client->hasSotTrackStatus(s);
        delete this;
    }
private:
    google::protobuf::Empty _request;
    google::protobuf::Empty _response;
    std::unique_ptr<core_ips::sot::Sot::Stub> _stub;
    grpc::ClientContext _context;
    GrpcClient* _client;
};


GrpcClient::GrpcClient(QObject* parent)
    : QObject(parent),
    _channel(nullptr) {
}

void GrpcClient::atStarted() {
    if (_channel) {
        _channel.reset();
    }
    _channel = grpc::CreateChannel("localhost:52124", grpc::InsecureChannelCredentials());
}

void GrpcClient::startTracking(const BBox &box) {
    qDebug() << "+== 1 " << QThread::currentThreadId();
    new GrpcSotTrackImpl(this, box);
}

void GrpcClient::stopTracking() {
    qDebug() << "+== 2 " << QThread::currentThreadId();
    new GrpcSotTrackStopImpl(this);
}

GrpcClient::~GrpcClient() {

}



