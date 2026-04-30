# Use pkg-config to dynamically fetch all necessary flags
LDFLAGS = $(shell pkg-config --libs grpc++ protobuf)
CXXFLAGS = -std=c++17 $(shell pkg-config --cflags grpc++)

# The rest of your Makefile
SRCS = src/main.cpp services.pb.cc services.grpc.pb.cc
OBJS = $(SRCS:.cpp=.o)

server_validator: $(SRCS)
	g++ $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o server_validator

clean:
	rm -f server_validator
