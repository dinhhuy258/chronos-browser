/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BLOCKERS_WORKER_H_
#define BLOCKERS_WORKER_H_

#include <string>
#include <vector>
#include <mutex>

class AdBlockClient;

namespace chronos {
namespace net {
namespace blockers {

class BlockersWorker {
 public:
  BlockersWorker();
  ~BlockersWorker();
  bool ShouldAdBlockUrl(const std::string& base_host, const std::string& url,
                        unsigned int resource_type);

 private:
  bool InitAdBlock();
  bool GetData(const char* file_name, std::vector<unsigned char>& buffer);

  std::vector<unsigned char> adblock_buffer_;
  AdBlockClient* adblock_parser_;
  std::mutex adblock_init_mutex_;
};

}  // namespace blockers
}  // namespace net
}  // namespace chronos

#endif // BLOCKERS_WORKER_H_
