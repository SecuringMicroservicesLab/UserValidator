cmake_minimum_required(VERSION 4.2)

# project name
project(cpp_service-1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# finding the system libraries
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# create the executable
add_executable(service-1_server
    src/main.cpp
    # your_file.pb.cc
    # your_file.grpc.pb.cc
)

# link the libraries to the executable
target_link_libraries(service-1_server
    protobuf::libprotobuf
    gRPC::grpc++
    gRPC::grpc++_reflection
)


