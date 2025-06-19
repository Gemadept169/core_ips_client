export PATH=/home/capybara/Downloads/grpc-release/release/bin:$PATH
export LD_LIBRARY_PATH=/home/capybara/Downloads/grpc-release/release/lib:$LD_LIBRARY_PATH
protoc -I. --cpp_out=../ --grpc_out=../ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./sot_service.proto
