#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // Required for std::find
#include <grpcpp/grpcpp.h>
#include "../services.grpc.pb.h"

using namespace grpc;
using namespace secure_lab;

class ValidatorServiceImpl final : public Validator::Service {
  private:
    // The database of valid users lives here so it persists across requests
    std::vector<std::string> valid_users_;

  public:
    // Constructor: Populate the vector when the server starts
    ValidatorServiceImpl()
    {
      valid_users_ = {"Owen", "Chris", "Akash"};
      std::cout << "[Validator] Database loaded with " << valid_users_.size() << " users." << std::endl;
    }

    // The RPC function called by the Orchestrator
    Status ValidateUser(ServerContext* context, const Name* nameRequest, ValidationResponse* validationReply) override
    {
      std::string incoming_name = nameRequest->name();
      std::cout << "[Validator] Checking access for: " << incoming_name << "... ";

      // Search the vector for the incoming name
      for (const std::string& user : valid_users_) 
      {
        if (user == incoming_name)
        {
          validationReply->set_is_valid(true);
          std::cout << "APPROVED." << std::endl;
          return Status::OK;
        }
      }
      //we did not find the user
      validationReply->set_is_valid(false);
      std::cout << "DENIED." << std::endl;
      // We always return Status::OK because the network call itself succeeded
      // even if the user was invalid
      return Status::OK;
    }
};

int main() {
  ValidatorServiceImpl service;
  ServerBuilder builder;

  // Listen on port 50051 (Locked inside the VPC, waiting for Orchestrator)
  std::string server_address("0.0.0.0:50051");
  builder.AddListeningPort(server_address, InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "[Validator] Server listening on " << server_address << std::endl;

  // Keep the server running indefinitely
  server->Wait();
  return 0;
}
