#include "FeaturedService.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include "FeatureIndex.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <time.h>
#include <iterator>
#include <list>
#include <vector>
#include <utility>
#include <stdint.h>
#include <math.h>
#include "boost/filesystem.hpp"

using namespace std;
using namespace boost::filesystem;

namespace featured {

class FeaturedServiceHandler : virtual public FeaturedServiceIf {
 private:
  FeatureIndex* features;  
  apache::thrift::server::TServer* server;

 public:
  FeaturedServiceHandler() {
  }

  FeaturedServiceHandler(FeatureIndex* features) {
    this->features = features;
  }

  void ping() {
    std::cout << "ping" << endl;
  }

  void shutdown() {
    server->stop();
  }

  void register_server(apache::thrift::server::TServer* server) {
    this->server = server;
  }

  void select(std::vector<Item> & _return, const Select& select_query) {
    features->select(_return, select_query);
  }

  void get_features(std::vector<std::string> & _return) {
    list<string> feature_names = features->get_feature_names();
    _return.insert(_return.end(), feature_names.begin(), feature_names.end());
  }

}; // namespace

}
list<path> get_files(path dir_path) {
  list<path> files;
  directory_iterator dir_end;
  for (directory_iterator iter(dir_path); iter != dir_end; ++iter) {
    if (is_directory(iter->status())) {
      list<path> subdir_files = get_files(iter->path());
      files.insert(files.end(), subdir_files.begin(), subdir_files.end());
    } else if (is_regular_file(iter->status())) {
      files.push_back(iter->path());
    }
  }
  return files;
}

int main(int argc, char **argv) {
  featured::FeatureIndex features;

  std::cout << "Welcome friend! I am featured server!" << endl;
  if (argc < 3) {
    std::cerr << "Usage: <port> <features_dir>" << endl;
    exit(1);
  }
  int port = atoi(argv[1]);

  string features_dir(argv[2]);
  if (!exists(features_dir)) {
    std::cerr << "Features path does not exist: " << features_dir << endl;
    exit(1);
  }
  path features_path(features_dir);
  list<path> feature_files = get_files(features_path);

  std::cout << "Loading features" << endl;
  std::cout << "Found " << feature_files.size() << " feature files in " << features_path << endl;
  for (list<path>::iterator iter = feature_files.begin(); iter != feature_files.end(); ++iter) {
    std::cout << "\tloading " <<*iter << endl;
    features.load(*iter);
  }
  map<string, uint> feature_sizes = features.get_feature_sizes();
  for (map<string, uint>::iterator iter = feature_sizes.begin(); iter != feature_sizes.end(); iter++) {
    std::cout << "\tloaded " << iter->second << " " << iter->first << endl;
  }
  std::cout << "loaded features for " << features.get_item_set_size() << " unique items" << endl;

  std::cout << "Finished building indexes" << endl;

  boost::shared_ptr<featured::FeaturedServiceHandler> handler(new featured::FeaturedServiceHandler(&features));
  boost::shared_ptr<apache::thrift::TProcessor> processor(new featured::FeaturedServiceProcessor(handler));
  boost::shared_ptr<apache::thrift::transport::TServerTransport> serverTransport(new apache::thrift::transport::TServerSocket(port));
  boost::shared_ptr<apache::thrift::transport::TTransportFactory> transportFactory(new apache::thrift::transport::TBufferedTransportFactory());
  boost::shared_ptr<apache::thrift::protocol::TProtocolFactory> protocolFactory(new apache::thrift::protocol::TBinaryProtocolFactory());
  std::cout << "Started featured server" << endl;
  apache::thrift::server::TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  handler->register_server(&server);
  server.serve();
  std::cout << "Stopped featured server, cleaning up" << endl;
  return 0;
}
