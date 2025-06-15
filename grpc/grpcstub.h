#ifndef GRPCSTUB_H
#define GRPCSTUB_H

#include <QObject>


class GrpcStub : public QObject
{
    Q_OBJECT
public:
    explicit GrpcStub(QObject *parent = nullptr);
    GrpcStub(const GrpcStub &) = delete;
    GrpcStub &operator=(const GrpcStub &) = delete;
    ~GrpcStub();

signals:
};

#endif // GRPCSTUB_H
