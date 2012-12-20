#!/usr/local/bin/thrift --gen cpp

namespace cpp featured

struct Item {
  1: i32 item_id,
  2: map<string, double> features,
}

struct Filter {
  1: required string feature,
  2: optional double coefficient,
  3: optional double upper_bound,
  4: optional double lower_bound,
}

struct Select {
  1: list<Filter> filters,
  2: optional string order_by_feature,
  3: i32 order_by_direction = -1, // Descending by default
  4: i32 limit = 10;
}

service FeaturedService {
  list<string> get_features(),
  list<Item> select(1: Select select_query),
  oneway void shutdown(),
  void ping(),
}
