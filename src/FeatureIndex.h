#ifndef FeatureIndex_H
#define FeatureIndex_H

#include "boost/filesystem.hpp"
#include <list>
#include <map>
#include <tr1/unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "featured_types.h"


namespace featured {

class FeatureIndex {
 public:
  virtual ~FeatureIndex() {}
  void load(boost::filesystem::path feature_path);
  std::list<std::string> get_feature_names();
  short int get_or_add_feature_id(const std::string & feature_name);
  void add_feature_value(const int32_t & item_id, const short int & feature_id, const double & value);
  double eval_item_score(const int32_t & item_id, const std::map<short int, double> & feature_coefficients);
  std::map<short int, double> get_optimized_feature_coefficients(std::map<std::string, double> feature_coefficients);
  void select(std::vector<Item> & _return, const featured::Select& select_query);
  std::map<std::string, uint> get_feature_sizes();
  uint get_item_set_size();
 private:
  short int get_feature_id(const std::string & feature_name);
  std::map<std::string, short int> feature_lookup;
  std::map<short int, std::string> reverse_feature_lookup;
  std::tr1::unordered_map<int32_t, std::tr1::unordered_map<short int, double> > item_feature_values;
  std::map<short int, uint> features_sizes;
};

} //namespace

#endif
