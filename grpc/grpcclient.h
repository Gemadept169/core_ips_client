#ifndef GRPCCLIENT_H
#define GRPCCLIENT_H

#include <QObject>
#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>
#include "api/sot_service.grpc.pb.h"
#include "common.h"
#include <memory>

class GrpcSotTrackImpl;
class GrpcSotTrackStopImpl;

class GrpcClient : public QObject {
    friend GrpcSotTrackImpl;
    friend GrpcSotTrackStopImpl;

    Q_OBJECT
public:
    explicit GrpcClient(QObject* parent = nullptr);
    GrpcClient(const GrpcClient &) = delete;
    GrpcClient &operator=(const GrpcClient &) = delete;
    ~GrpcClient();

signals:
    void hasSotTrackNewResponse(const SotInfo&);
    void hasSotTrackStatus(const grpc::Status&);

public slots:
    void atStarted();
    void startTracking(const BBox& box);
    void stopTracking();

private:
    std::shared_ptr<grpc::Channel> _channel;
};

#endif // GRPCCLIENT_H
