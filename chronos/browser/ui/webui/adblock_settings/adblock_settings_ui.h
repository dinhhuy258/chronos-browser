#ifndef CHRONOS_BROWSER_UI_WEBUI_ADBLOCK_SETTINGS_ADBLOCK_SETTINGS_UI_H_
#define CHRONOS_BROWSER_UI_WEBUI_ADBLOCK_SETTINGS_ADBLOCK_SETTINGS_UI_H_

#include "base/files/file_path.h"
#include "base/macros.h"
#include "content/public/browser/web_ui_controller.h"

namespace base {
  class ListValue;
} // namespace base

namespace content {
  class WebUIDataSource;
} // namespace content

// The WebUI for adblock-settings
class AdblockSettingsUI : public content::WebUIController {
 public:
  explicit AdblockSettingsUI(content::WebUI* web_ui);
  ~AdblockSettingsUI() override;
 private:
  // Setting adblock (enable or disable)
  void SettingAdblock(const base::ListValue* args);
  // Setting popup blocker
  void SettingPopupBlocker(const base::ListValue* args);

  base::FilePath popup_blocker_setting_file_path_;
  content::WebUIDataSource* source_;
  DISALLOW_COPY_AND_ASSIGN(AdblockSettingsUI);
};

#endif // CHRONOS_BROWSER_UI_WEBUI_ADBLOCK_SETTINGS_ADBLOCK_SETTINGS_UI_H
