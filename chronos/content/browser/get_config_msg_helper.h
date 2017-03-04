#ifndef CHRONOS_CONTENT_BROWSER_GET_CONFIG_MSG_HELPER_H_
#define CHRONOS_CONTENT_BROWSER_GET_CONFIG_MSG_HELPER_H_

#include "content/common/content_export.h"
#include "content/public/browser/browser_message_filter.h"

namespace chronos {
namespace content {

class CONTENT_EXPORT GetConfigMsgHelper : public ::content::BrowserMessageFilter {
 public:
  explicit GetConfigMsgHelper();

  // BrowserMessageFilter implementation
  bool OnMessageReceived(const IPC::Message& message) override;

  void GetPopupBlockerSetting(int* popupBlockerSetting);

 protected:
  ~GetConfigMsgHelper() override;
};

} // namespace content
} // namespace chronos

#endif // CHRONOS_CONTENT_BROWSER_GET_CONFIG_MSG_HELPER_H_
