#ifndef SOTCLIENT_H
#define SOTCLIENT_H

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <QMetaObject>
#include <QObject>
#include <memory>

#include "api/sot_service.grpc.pb.h"
#include "common.h"
#include "grpcclient.h"

class SotClient {
   public:
    explicit SotClient(std::shared_ptr<grpc::Channel> channel, GrpcClient* client);
    SotClient(const SotClient&) = delete;
    SotClient& operator=(const SotClient&) = delete;
    ~SotClient();

    void track(const BBox& initBox);

   private:
    std::unique_ptr<core_ips::sot::Sot::Stub> _stub;
    std::vector<core_ips::sot::TrackResponse> _responceList;
    GrpcClient* _client;
};

#endif  // SOTCLIENT_H
