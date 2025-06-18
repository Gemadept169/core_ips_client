#include "sotclient.h"

#include <QDebug>
#include <QMetaType>
#include <condition_variable>
#include <mutex>

class SotReader : public grpc::ClientReadReactor<core_ips::sot::TrackResponse> {
   public:
    SotReader(core_ips::sot::Sot::Stub* stub, GrpcClient* client, const BBox& box)
        : _client(client),
          _isDone(false) {
        _request.mutable_init_bbox()->set_xtl(box.xtl);
        _request.mutable_init_bbox()->set_ytl(box.ytl);
        _request.mutable_init_bbox()->set_width(box.width);
        _request.mutable_init_bbox()->set_height(box.height);
        stub->async()->Track(&_context, &_request, this);
        StartRead(&_responce);
        StartCall();
    }
    void OnReadDone(bool ok) override {
        if (ok && _client) {
            SotInfo incomingInfo;
            qDebug() << "[SotReader::OnReadDone] Incoming info has state enum: " << _responce.state();
            incomingInfo.bbox.xtl = _responce.result().bbox().xtl();
            incomingInfo.bbox.ytl = _responce.result().bbox().ytl();
            incomingInfo.bbox.width = _responce.result().bbox().width();
            incomingInfo.bbox.height = _responce.result().bbox().height();
            incomingInfo.score = _responce.result().score();
            QMetaObject::invokeMethod(_client,
                                      "hasSotTrackNewResponse",
                                      Qt::QueuedConnection,
                                      Q_ARG(SotInfo, incomingInfo));
            StartRead(&_responce);
        }
    }
    void OnDone(const grpc::Status& s) override {
        std::unique_lock<std::mutex> l(_mu);
        _status = s;
        _isDone = true;
        _cv.notify_one();
    }
    grpc::Status Await() {
        std::unique_lock<std::mutex> l(_mu);
        _cv.wait(l, [this] { return _isDone; });
        return std::move(_status);
    }

   private:
    grpc::ClientContext _context;
    GrpcClient* _client;
    core_ips::sot::TrackRequest _request;
    core_ips::sot::TrackResponse _responce;
    std::mutex _mu;
    std::condition_variable _cv;
    grpc::Status _status;
    bool _isDone;
};

SotClient::SotClient(std::shared_ptr<grpc::Channel> channel, GrpcClient* client)
    : _stub(core_ips::sot::Sot::NewStub(channel)),
      _client(client) {
}

SotClient::~SotClient() {
}

void SotClient::track(const BBox& bbox) {
    SotReader reader(_stub.get(), _client, bbox);
    grpc::Status status = reader.Await();
    if (status.ok()) {
        std::cout << "[SotClient::track] Track rpc succeeded." << std::endl;
    } else {
        std::cout << "[SotClient::track] Track rpc failed." << std::endl;
    }
}
