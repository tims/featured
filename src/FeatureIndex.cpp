
#include "FeatureIndex.h"
#include <utility>

namespace featured {

short int FeatureIndex::get_or_add_feature_id(const std::string & feature_name) {
  short int feature_id = 0;
  if (feature_lookup.find(feature_name) != feature_lookup.end()) {
    feature_id = feature_lookup[feature_name];
  } else {
    short int max_int = 0;
    for (std::map<std::string,short int>::iterator iter = feature_lookup.begin(); iter != feature_lookup.end(); iter++) {
      max_int = std::max(max_int, iter->second);
    }
    feature_id = max_int + 1;
    feature_lookup[feature_name] = feature_id;
    reverse_feature_lookup[feature_id] = feature_name;
  }
  return feature_id;
}

short int FeatureIndex::get_feature_id(const std::string & feature_name) {
  short int feature_id;
  if (feature_lookup.find(feature_name) != feature_lookup.end()) {
    feature_id = feature_lookup[feature_name];
  } else {
    throw std::invalid_argument("unknown feature " + feature_name);
  }
  return feature_id;
}

std::list<std::string> FeatureIndex::get_feature_names() {
  std::list<std::string> feature_names;
  for (std::map<std::string, short int>::iterator iter = feature_lookup.begin(); iter != feature_lookup.end(); ++iter) {
    feature_names.push_back(iter->first);
  }
  return feature_names;
}

void FeatureIndex::add_feature_value(const int32_t & item_id, const short int & feature_id, const double & value) {
  item_feature_values[item_id][feature_id] = value;
  features_sizes[feature_id]++;
}

void FeatureIndex::load(boost::filesystem::path feature_path) {
  std::ifstream feature_file(feature_path.string().c_str());
  std::string feature_name(feature_path.stem().c_str());

  short int feature_id = get_or_add_feature_id(feature_name);

  std::stringstream ss;
  std::string line;
  long load_count = 0;
  int32_t item_id;
  double value;
  while (feature_file) {
    getline(feature_file, line);
    ss.clear();
    ss.str(line);
    ss >> item_id >> value;
    add_feature_value(item_id, feature_id, value);
  }
}

std::map<std::string, uint> FeatureIndex::get_feature_sizes() {
  std::map<std::string, uint> named_features_sizes;
  for (std::map<std::string, short int>::iterator iter = feature_lookup.begin(); iter != feature_lookup.end(); iter++) {
    named_features_sizes[iter->first] = features_sizes[iter->second];
  }
  return named_features_sizes;
}

uint FeatureIndex::get_item_set_size() {
  return item_feature_values.size();
}


std::map<short int, double> FeatureIndex::get_optimized_feature_coefficients(std::map<std::string, double> feature_coefficients) {
  std::map<short int, double> optimized_feature_coefficients;
  std::map<std::string, double>::iterator iter;
  for (iter = feature_coefficients.begin(); iter != feature_coefficients.end(); ++iter) {
    short int feature_id = get_feature_id(iter->first);
    optimized_feature_coefficients[feature_id] = iter->second;
  }
  return optimized_feature_coefficients;
}

double FeatureIndex::eval_item_score(const int32_t & item_id, const std::map<short int, double> & feature_coefficients) {
  std::tr1::unordered_map<short int, double> feature_values = item_feature_values[item_id];
  double result = 0;
  std::map<short int, double>::const_iterator coefficients_iter;
  for (coefficients_iter = feature_coefficients.begin(); coefficients_iter != feature_coefficients.end(); ++coefficients_iter) {
    result += coefficients_iter->second * feature_values[coefficients_iter->first];
  }
  return result;
}

void FeatureIndex::select(std::vector<Item> & _return, const Select& select_query) {
  std::tr1::unordered_map<int32_t, std::tr1::unordered_map<short int, double> >::iterator items_iter;
  std::multimap<double,int32_t> sorted_items;
  bool ordered = select_query.__isset.order_by_feature;
  short int order_by_feature_id;
  if (ordered) {
    order_by_feature_id = get_feature_id(select_query.order_by_feature);
  }

  for (items_iter = item_feature_values.begin(); items_iter != item_feature_values.end(); items_iter++) {
    std::tr1::unordered_map<short int, double> * values = &(items_iter->second);
    bool filtered = false;
    for (std::vector<Filter>::const_iterator filter_iter = select_query.filters.begin(); filter_iter != select_query.filters.end(); filter_iter++) {
      short int feature_id = get_feature_id(filter_iter->feature);
      if (filter_iter->__isset.upper_bound && (*values)[feature_id] > filter_iter->upper_bound) {
        filtered = true;
      } else if (filter_iter->__isset.lower_bound && (*values)[feature_id] < filter_iter->lower_bound) {
        filtered = true;
      }
    }
    if (!filtered) {
      if (ordered) {
        sorted_items.insert(std::pair<double, int32_t>((*values)[order_by_feature_id], items_iter->first));
      } else {
        Item item;
        item.__set_item_id(items_iter->first);
        _return.push_back(item);
        if (_return.size() >= select_query.limit) {
          break;
        }
      }
    }
  }

  if (ordered) {
    if (select_query.order_by_direction < 0) {
      for (std::multimap<double,int32_t>::reverse_iterator iter = sorted_items.rbegin(); iter != sorted_items.rend() && _return.size() < select_query.limit; iter++) {
        Item item;
        item.__set_item_id(iter->second);
        _return.push_back(item);
      }
    } else {
      for (std::multimap<double,int32_t>::iterator iter = sorted_items.begin(); iter != sorted_items.end() && _return.size() < select_query.limit; iter++) {
        Item item;
        item.__set_item_id(iter->second);
         _return.push_back(item);
      }
    }
  }

  std::vector<short int> filter_feature_ids;
  for (std::vector<Filter>::const_iterator filter_iter = select_query.filters.begin(); filter_iter != select_query.filters.end(); filter_iter++) {
    short int feature_id = get_feature_id(filter_iter->feature);
    filter_feature_ids.push_back(feature_id);
  }
  for (std::vector<Item>::iterator items_iter = _return.begin(); items_iter != _return.end(); items_iter++) {
    std::tr1::unordered_map<short int, double> * feature_values = &item_feature_values[items_iter->item_id];
    std::map<std::string, double> features;
    for (std::vector<short int>::iterator feature_ids_iter = filter_feature_ids.begin(); feature_ids_iter != filter_feature_ids.end(); feature_ids_iter++) {
      short int feature_id = *feature_ids_iter;
      std::string feature_name = reverse_feature_lookup[feature_id];
      double feature_value = (*feature_values)[feature_id];
      features[feature_name] = feature_value;
    }
    if (ordered) {
      std::string feature_name = reverse_feature_lookup[order_by_feature_id];
      double feature_value = (*feature_values)[order_by_feature_id];
      features[feature_name] = feature_value;
    }
    items_iter->__set_features(features);
  }
}

} //namespace

