export PATH=$PATH:/home/thai/Releases/grpc/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/thai/Releases/grpc/lib
protoc -I. --cpp_out=../ --grpc_out=../ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./sot_service.proto
