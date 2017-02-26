#include "chronos/browser/ui/webui/adblock_settings/adblock_settings_ui.h"

#include "base/values.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/url_constants.h"
#include "chrome/grit/browser_resources.h"
#include "components/strings/grit/components_strings.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/browser/web_ui.h"

namespace {

  enum AdblockSetting {
    DISABLE_ADBLOCK = 0,
    ENABLE_ADBLOCK = 1,
    ENABLE_SMART_ADBLOCK = 2
  };

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
  source =content::WebUIDataSource::Create(
      chrome::kChronosUIAdblockSettingsFrameHost);
  // Register callback handler
  web_ui->RegisterMessageCallback("settingAdblock",
                                  base::Bind(&AdblockSettingsUI::SettingAdblock,
                                             base::Unretained(this)));
  // Add required info
  source->AddLocalizedString("adblockSettingsTitle",
                              IDS_ADBLOCK_SETTINGS_TITLE);
  bool is_adblock_enabled = GetPrefService()->GetBoolean(prefs::kEnableAdBlock);
  bool is_smart_adblock_enabled =
      GetPrefService()->GetBoolean(prefs::kEnableSmartAdBlock);
  int value = DISABLE_ADBLOCK;
  if (is_adblock_enabled) {
    value = ENABLE_ADBLOCK;
  }
  else if (is_smart_adblock_enabled) {
    value = ENABLE_SMART_ADBLOCK;
  }
  source->AddInteger("adblockSetting", value);
  source->SetJsonPath("strings.js");
  // Add required resources
  source->AddResourcePath("uber_shared.css", IDR_UBER_SHARED_CSS);
  source->AddResourcePath("adblock_settings.js", IDR_ADBLOCK_SETTINGS_JS);
  source->SetDefaultResource(IDR_ADBLOCK_SETTINGS_HTML);
  // Disable deny X frame so that we can show it as a uber frame
  source->DisableDenyXFrameOptions();

  Profile* profile = Profile::FromWebUI(web_ui);
  content::WebUIDataSource::Add(profile, source);
}

AdblockSettingsUI::~AdblockSettingsUI() {
}

void AdblockSettingsUI::SettingAdblock(const base::ListValue* args) {
  int value;
  if (!args->GetInteger(0, &value)) {
    return;
  }
  switch (value) {
    case DISABLE_ADBLOCK:
      GetPrefService()->SetBoolean(prefs::kEnableAdBlock, false);
      GetPrefService()->SetBoolean(prefs::kEnableSmartAdBlock, false);
      break;
    case ENABLE_ADBLOCK:
      GetPrefService()->SetBoolean(prefs::kEnableAdBlock, true);
      GetPrefService()->SetBoolean(prefs::kEnableSmartAdBlock, false);
      break;
    case ENABLE_SMART_ADBLOCK:
      GetPrefService()->SetBoolean(prefs::kEnableAdBlock, false);
      GetPrefService()->SetBoolean(prefs::kEnableSmartAdBlock, true);
      break;
  }
  source->AddInteger("adblockSetting", value);
}
