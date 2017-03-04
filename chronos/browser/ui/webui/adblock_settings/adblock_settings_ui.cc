#include "chronos/browser/ui/webui/adblock_settings/adblock_settings_ui.h"

#include "base/files/file_util.h"
#include "base/path_service.h"
#include "base/values.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/url_constants.h"
#include "chrome/grit/browser_resources.h"
#include "chronos/constants/chronos_constants.h"
#include "components/strings/grit/components_strings.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/browser/web_ui.h"


namespace {

  Profile* GetOriginalProfile() {
    return ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
  }

  PrefService* GetPrefService() {
    return GetOriginalProfile()->GetPrefs();
  }

} // namespace

AdblockSettingsUI::AdblockSettingsUI(content::WebUI* web_ui)
    : content::WebUIController(web_ui) {
  // Set up the adblock-settings source.
  source_ =content::WebUIDataSource::Create(
      chrome::kChronosUIAdblockSettingsFrameHost);
  // Register callback handler
  web_ui->RegisterMessageCallback("settingAdblock",
                                  base::Bind(&AdblockSettingsUI::SettingAdblock,
                                             base::Unretained(this)));

  web_ui->RegisterMessageCallback("settingPopupBlocker", base::Bind(
                                      &AdblockSettingsUI::SettingPopupBlocker,
                                      base::Unretained(this)));
  // Add required info
  source_->AddLocalizedString("adblockSettingsTitle",
                              IDS_ADBLOCK_SETTINGS_TITLE);
  bool is_adblock_enabled = GetPrefService()->GetBoolean(prefs::kEnableAdBlock);
  bool is_smart_adblock_enabled =
      GetPrefService()->GetBoolean(prefs::kEnableSmartAdBlock);
  char adblockSetting = chronos::constants::DISABLE_ADBLOCK;
  if (is_adblock_enabled) {
    adblockSetting = chronos::constants::ENABLE_ADBLOCK;
  }
  else if (is_smart_adblock_enabled) {
    adblockSetting = chronos::constants::ENABLE_SMART_ADBLOCK;
  }
    source_->AddInteger(chronos::constants::kAdblockSetting, adblockSetting);

  base::FilePath dir_user_data_path;
  PathService::Get(chrome::DIR_USER_DATA, &dir_user_data_path);
  char popupBlockerSetting = chronos::constants::DISABLE_POPUP_BLOCKER;
  // Hard code
  popup_blocker_setting_file_path_ = dir_user_data_path.
      Append(FILE_PATH_LITERAL(chronos::constants::kPopupBlockerSetting));
  if (base::PathExists(popup_blocker_setting_file_path_)) {
    base::ReadFile(popup_blocker_setting_file_path_, &popupBlockerSetting, 1);
  }
  source_->AddInteger(chronos::constants::kPopupBlockerSetting,
                      popupBlockerSetting);
  source_->SetJsonPath("strings.js");
  // Add required resources
  source_->AddResourcePath("uber_shared.css", IDR_UBER_SHARED_CSS);
  source_->AddResourcePath("adblock_settings.js", IDR_ADBLOCK_SETTINGS_JS);
  source_->SetDefaultResource(IDR_ADBLOCK_SETTINGS_HTML);
  // Disable deny X frame so that we can show it as a uber frame
  source_->DisableDenyXFrameOptions();

  Profile* profile = Profile::FromWebUI(web_ui);
  content::WebUIDataSource::Add(profile, source_);
}

AdblockSettingsUI::~AdblockSettingsUI() {
}

void AdblockSettingsUI::SettingAdblock(const base::ListValue* args) {
  int value;
  if (!args->GetInteger(0, &value)) {
    return;
  }
  switch (value) {
    case chronos::constants::DISABLE_ADBLOCK:
      GetPrefService()->SetBoolean(prefs::kEnableAdBlock, false);
      GetPrefService()->SetBoolean(prefs::kEnableSmartAdBlock, false);
      break;
    case chronos::constants::ENABLE_ADBLOCK:
      GetPrefService()->SetBoolean(prefs::kEnableAdBlock, true);
      GetPrefService()->SetBoolean(prefs::kEnableSmartAdBlock, false);
      break;
    case chronos::constants::ENABLE_SMART_ADBLOCK:
      GetPrefService()->SetBoolean(prefs::kEnableAdBlock, false);
      GetPrefService()->SetBoolean(prefs::kEnableSmartAdBlock, true);
      break;
  }
  source_->AddInteger(chronos::constants::kAdblockSetting, value);
}

void AdblockSettingsUI::SettingPopupBlocker(const base::ListValue* args) {
  int value;
  if (!args->GetInteger(0, &value)) {
    return;
  }
  /*
   * Currently this way is a bad design. Because popup blocker happens in
   * renderer so I saved the value setting in config file, and in renderer I
   * read the setting config in this file.
   * In the future I will refactor it when I understand chromium system clearly
   */
  char popupBlockerSetting = static_cast<char>(value);
  if (base::WriteFile(popup_blocker_setting_file_path_, &popupBlockerSetting, 1)
      != -1) {
    source_->AddInteger(chronos::constants::kPopupBlockerSetting, value);
  }
}
