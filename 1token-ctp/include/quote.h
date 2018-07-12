#pragma once
#include <unordered_map>
#include "error_code.h"
#include "onetoken_interface.h"
#include "rapidjson/document.h"
#include "structs.h"

namespace onetoken {
class Quote {
 public:
  virtual ErrorCode ParseTickData(const rapidjson::Value &data,
                                  MarketResponseMessage &message);
  virtual ErrorCode ParseZhubiData(const rapidjson::Value &data,
                                   MarketResponseMessage &message);
  virtual void HandleError(ErrorCode error_code, const std::string &info);

  void SetUserInterface(UserInterface *user_interface) {
    user_interface_ = user_interface;
  }

 protected:
  UserInterface *user_interface_;
  std::unordered_map<std::string, uint32_t> contract_seq_map_;
  ReqType req_type_;
};
}  // namespace onetoken