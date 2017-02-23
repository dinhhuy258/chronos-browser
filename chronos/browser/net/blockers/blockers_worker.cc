/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "blockers_worker.h"

#include "../../../../base/threading/thread_restrictions.h"
#include "../../../../base/files/file_util.h"
#include "../../../../base/path_service.h"
#include "../../../../chrome/common/chrome_paths.h"
#include "../../../../content/public/common/resource_type.h"

#include "ad_block_client.h"

#define kAdBlockDataFile "abp_filter_parser.dat"

namespace chronos {
namespace net {
namespace blockers {

  BlockersWorker::BlockersWorker() : adblock_parser_(nullptr) {
    base::ThreadRestrictions::SetIOAllowed(true);
  }

  BlockersWorker::~BlockersWorker() {
    if (adblock_parser_ != nullptr) {
      delete adblock_parser_;
    }
  }

  bool BlockersWorker::InitAdBlock() {
    std::lock_guard<std::mutex> guard(adblock_init_mutex_);

    if (adblock_parser_) {
        return true;
    }

    if (!GetData(kAdBlockDataFile, adblock_buffer_)) {
        return false;
    }

    adblock_parser_ = new AdBlockClient();
    if (!adblock_parser_->deserialize((char*)&adblock_buffer_.front())) {
        delete adblock_parser_;
        adblock_parser_ = nullptr;
        LOG(ERROR) << "Adblock deserialize failed";
        return false;
    }

    return true;
  }

  bool BlockersWorker::GetData(const char* file_name,
                               std::vector<unsigned char>& buffer) {
    base::FilePath data_file_path;
    base::FilePath dir_user_data_path;
    int64_t size = 0;

    if (!PathService::Get(chrome::DIR_RESOURCES, &dir_user_data_path)) {
      LOG(ERROR) << "Can not get dir resources path";
      return false;
    }

    data_file_path = dir_user_data_path.Append(FILE_PATH_LITERAL(file_name));
    if (!base::PathExists(data_file_path) ||
        !base::GetFileSize(data_file_path, &size) || size == 0) {
      LOG(ERROR) << "The dat file is corrupted: " << file_name;
      return false;
    }

    std::vector<char> data(size + 1);
    if (size != base::ReadFile(data_file_path, (char*)&data.front(), size)) {
      LOG(ERROR) << "Can not read dat info file " << file_name;
      return false;
    }

    data[size] = '\0';
    buffer.resize(size + 1);
    ::memcpy(&buffer.front(), &data.front(), size + 1);
    return true;
  }

  bool BlockersWorker::ShouldAdBlockUrl(const std::string& base_host,
                                        const std::string& url,
                                        unsigned int resource_type) {
    if (!InitAdBlock()) {
      return false;
    }

    FilterOption currentOption = FONoFilterOption;
    content::ResourceType internalResource =
                          (content::ResourceType)resource_type;
    if (content::RESOURCE_TYPE_STYLESHEET == internalResource) {
      currentOption = FOStylesheet;
    } else if (content::RESOURCE_TYPE_IMAGE == internalResource) {
      currentOption = FOImage;
    } else if (content::RESOURCE_TYPE_SCRIPT == internalResource) {
      currentOption = FOScript;
    }

    return adblock_parser_->matches(url.c_str(), currentOption,
                                    base_host.c_str());
  }

}  // namespace blockers
}  // namespace net
}  // namespace chronos
