#include "chronos/content/browser/get_config_msg_helper.h"

#include "base/files/file_util.h"
#include "base/path_service.h"
#include "chronos/constants/chronos_constants.h"
#include "chrome/common/chrome_paths.h"
#include "content/common/view_messages.h"

namespace chronos {
namespace content {

GetConfigMsgHelper::GetConfigMsgHelper() : BrowserMessageFilter(ViewMsgStart) {
}

GetConfigMsgHelper::~GetConfigMsgHelper() {
}

bool GetConfigMsgHelper::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(GetConfigMsgHelper, message)
    IPC_MESSAGE_HANDLER(ViewHostMsg_GetPopupBlockerSetting,
                        GetPopupBlockerSetting)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}

void GetConfigMsgHelper::GetPopupBlockerSetting(int* popupBlockerSetting) {
  base::FilePath dir_user_data_path, popup_blocker_setting_file_path;
  PathService::Get(chrome::DIR_USER_DATA, &dir_user_data_path);
  char value = chronos::constants::DISABLE_POPUP_BLOCKER;
  popup_blocker_setting_file_path = dir_user_data_path.
      Append(FILE_PATH_LITERAL(chronos::constants::kPopupBlockerSetting));
  if (base::PathExists(popup_blocker_setting_file_path)) {
    base::ReadFile(popup_blocker_setting_file_path, &value, 1);
  }
  *popupBlockerSetting = value;
}

} // namespace content
} // namespace chronos
