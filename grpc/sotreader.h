#ifndef SOTREADER_H
#define SOTREADER_H

#include "grpc/grpc.h"
#include "api/sot_service.grpc.pb.h"

class SotReader : public grpc::ClientReadReactor<core_ips::sot::TrackResponse> {
public:
    explicit SotReader();
    SotReader(const SotReader &) = delete;
    SotReader &operator=(const SotReader &) = delete;
    ~SotReader();

private:

};

#endif // SOTREADER_H
