#ifndef SOTCLIENT_H
#define SOTCLIENT_H

#include <memory>
#include <string>
#include <grpcpp/channel.h>
#include "api/sot_service.grpc.pb.h"
#include <QMetaObject>
#include "common.h"

class SotClient {
public:
    explicit SotClient(std::shared_ptr<grpc::Channel> channel);
    SotClient(const SotClient &) = delete;
    SotClient &operator=(const SotClient &) = delete;
    ~SotClient();

    void track(const BBox& bbox);

private:
    std::unique_ptr<core_ips::sot::Sot::Stub> _stub;
    std::vector<core_ips::sot::TrackResponse> _responceList;
};

#endif // SOTCLIENT_H
