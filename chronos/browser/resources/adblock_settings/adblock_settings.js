var AdblockSettings = {
  DISABLE_ADBLOCK : 0,
  ENABLE_ADBLOCK : 1,
  ENABLE_SMART_ADBLOCK : 2,
}

cr.define('adblock_settings', function() {
  'use strict';

  function initialize() {
    var adblockSetting = loadTimeData.getInteger('adblockSetting');
    if (adblockSetting == AdblockSettings.DISABLE_ADBLOCK) {
      $('disable_adblock').checked = true;
    }
    else if (adblockSetting == AdblockSettings.ENABLE_ADBLOCK) {
      $('enable_adblock').checked = true;
    }
    else if (adblockSetting == AdblockSettings.ENABLE_SMART_ADBLOCK){
      $('enable_smart_adblock').checked = true;
    }

    $('disable_adblock').onclick = function() {
      adblock_settings.settingAdblock(AdblockSettings.DISABLE_ADBLOCK);
    };
    $('enable_adblock').onclick = function() {
      adblock_settings.settingAdblock(AdblockSettings.ENABLE_ADBLOCK);
    };
    $('enable_smart_adblock').onclick = function() {
      adblock_settings.settingAdblock(AdblockSettings.ENABLE_SMART_ADBLOCK);
    };
  }

  function settingAdblock(value) {
    chrome.send("settingAdblock", [value])
  }

  // Return an object with all of the exports.
  return {
    settingAdblock: settingAdblock,
    initialize: initialize,
  };
});

document.addEventListener('DOMContentLoaded', adblock_settings.initialize);
