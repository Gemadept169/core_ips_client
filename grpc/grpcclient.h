#ifndef GRPCCLIENT_H
#define GRPCCLIENT_H

#include <QObject>
#include <QMetaObject>
#include "common.h"

class SotClient;
class GrpcClient : public QObject {
    Q_OBJECT
public:
    explicit GrpcClient(QObject* parent = nullptr);
    GrpcClient(const GrpcClient &) = delete;
    GrpcClient &operator=(const GrpcClient &) = delete;
    ~GrpcClient();

signals:
    void hasSotTrackNewResponse(const SotInfo&);

public slots:
    void atStarted();

private:
    SotClient *_sotClient;

};

#endif // GRPCCLIENT_H
