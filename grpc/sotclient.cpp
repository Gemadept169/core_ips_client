#include "sotclient.h"

namespace {
    class SotReader : public grpc::ClientReadReactor<core_ips::sot::TrackResponse> {
        public:
            SotReader(core_ips::sot::Sot::Stub* stub, const BBox& box)
                : _initBox(box) {
                _request
                stub->async()->Track(&_context, &_initBox, this);
                StartRead(&feature_);
                StartCall();
            }
            void OnReadDone(bool ok) override {
                if (ok) {
                    std::cout << "Found feature called " << feature_.name() << " at "
                              << feature_.location().latitude() / coord_factor_ << ", "
                              << feature_.location().longitude() / coord_factor_
                              << std::endl;
                    StartRead(&feature_);
                }
            }
            void OnDone(const Status& s) override {
                std::unique_lock<std::mutex> l(mu_);
                status_ = s;
                done_ = true;
                cv_.notify_one();
            }
            Status Await() {
                std::unique_lock<std::mutex> l(mu_);
                cv_.wait(l, [this] { return done_; });atSotResults(sot::SotInfo)
                return std::move(status_);
            }

        private:
            grpc::ClientContext _context;
            core_ips::sot::TrackRequest _request;
            std::mutex mu_;
            std::condition_variable cv_;
            Status status_;
            bool done_ = false;
    };
}

SotClient::SotClient(std::shared_ptr<grpc::Channel> channel)
        : _stub(core_ips::sot::Sot::NewStub(channel)) {
}

SotClient::~SotClient() {
}

void SotClient::track(const BBox &bbox) {

}


