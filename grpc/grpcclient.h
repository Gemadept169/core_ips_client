#ifndef GRPCCLIENT_H
#define GRPCCLIENT_H

#include "grpc/grpc.h"
#include "api/sot_service.grpc.pb.h"

class GrpcClient : public grpc::ClientReadReactor<core_ips::sot::TrackResponse> {
public:
    explicit GrpcClient();
    GrpcClient(const GrpcClient &) = delete;
    GrpcClient &operator=(const GrpcClient &) = delete;
    ~GrpcClient();

private:

};

#endif // GRPCCLIENT_H
