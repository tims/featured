/**
 * Autogenerated by Thrift Compiler (0.8.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef featured_TYPES_H
#define featured_TYPES_H

#include <Thrift.h>
#include <TApplicationException.h>
#include <protocol/TProtocol.h>
#include <transport/TTransport.h>



namespace featured {

typedef struct _Item__isset {
  _Item__isset() : item_id(false), features(false) {}
  bool item_id;
  bool features;
} _Item__isset;

class Item {
 public:

  static const char* ascii_fingerprint; // = "D532D9A2E8F3A7C0DF088B9D42C44294";
  static const uint8_t binary_fingerprint[16]; // = {0xD5,0x32,0xD9,0xA2,0xE8,0xF3,0xA7,0xC0,0xDF,0x08,0x8B,0x9D,0x42,0xC4,0x42,0x94};

  Item() : item_id(0) {
  }

  virtual ~Item() throw() {}

  int32_t item_id;
  std::map<std::string, double>  features;

  _Item__isset __isset;

  void __set_item_id(const int32_t val) {
    item_id = val;
  }

  void __set_features(const std::map<std::string, double> & val) {
    features = val;
  }

  bool operator == (const Item & rhs) const
  {
    if (!(item_id == rhs.item_id))
      return false;
    if (!(features == rhs.features))
      return false;
    return true;
  }
  bool operator != (const Item &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Item & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Filter__isset {
  _Filter__isset() : coefficient(false), upper_bound(false), lower_bound(false) {}
  bool coefficient;
  bool upper_bound;
  bool lower_bound;
} _Filter__isset;

class Filter {
 public:

  static const char* ascii_fingerprint; // = "BBD19E3475365A44E56C102645D1F2B2";
  static const uint8_t binary_fingerprint[16]; // = {0xBB,0xD1,0x9E,0x34,0x75,0x36,0x5A,0x44,0xE5,0x6C,0x10,0x26,0x45,0xD1,0xF2,0xB2};

  Filter() : feature(""), coefficient(0), upper_bound(0), lower_bound(0) {
  }

  virtual ~Filter() throw() {}

  std::string feature;
  double coefficient;
  double upper_bound;
  double lower_bound;

  _Filter__isset __isset;

  void __set_feature(const std::string& val) {
    feature = val;
  }

  void __set_coefficient(const double val) {
    coefficient = val;
    __isset.coefficient = true;
  }

  void __set_upper_bound(const double val) {
    upper_bound = val;
    __isset.upper_bound = true;
  }

  void __set_lower_bound(const double val) {
    lower_bound = val;
    __isset.lower_bound = true;
  }

  bool operator == (const Filter & rhs) const
  {
    if (!(feature == rhs.feature))
      return false;
    if (__isset.coefficient != rhs.__isset.coefficient)
      return false;
    else if (__isset.coefficient && !(coefficient == rhs.coefficient))
      return false;
    if (__isset.upper_bound != rhs.__isset.upper_bound)
      return false;
    else if (__isset.upper_bound && !(upper_bound == rhs.upper_bound))
      return false;
    if (__isset.lower_bound != rhs.__isset.lower_bound)
      return false;
    else if (__isset.lower_bound && !(lower_bound == rhs.lower_bound))
      return false;
    return true;
  }
  bool operator != (const Filter &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Filter & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Select__isset {
  _Select__isset() : filters(false), order_by_feature(false), order_by_direction(false), limit(false) {}
  bool filters;
  bool order_by_feature;
  bool order_by_direction;
  bool limit;
} _Select__isset;

class Select {
 public:

  static const char* ascii_fingerprint; // = "16D2D6F84E1846A93F12E0482A140D8A";
  static const uint8_t binary_fingerprint[16]; // = {0x16,0xD2,0xD6,0xF8,0x4E,0x18,0x46,0xA9,0x3F,0x12,0xE0,0x48,0x2A,0x14,0x0D,0x8A};

  Select() : order_by_feature(""), order_by_direction(-1), limit(10) {
  }

  virtual ~Select() throw() {}

  std::vector<Filter>  filters;
  std::string order_by_feature;
  int32_t order_by_direction;
  int32_t limit;

  _Select__isset __isset;

  void __set_filters(const std::vector<Filter> & val) {
    filters = val;
  }

  void __set_order_by_feature(const std::string& val) {
    order_by_feature = val;
    __isset.order_by_feature = true;
  }

  void __set_order_by_direction(const int32_t val) {
    order_by_direction = val;
  }

  void __set_limit(const int32_t val) {
    limit = val;
  }

  bool operator == (const Select & rhs) const
  {
    if (!(filters == rhs.filters))
      return false;
    if (__isset.order_by_feature != rhs.__isset.order_by_feature)
      return false;
    else if (__isset.order_by_feature && !(order_by_feature == rhs.order_by_feature))
      return false;
    if (!(order_by_direction == rhs.order_by_direction))
      return false;
    if (!(limit == rhs.limit))
      return false;
    return true;
  }
  bool operator != (const Select &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Select & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

} // namespace

#endif