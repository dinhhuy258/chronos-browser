var AdblockSettings = {
  DISABLE_ADBLOCK : 0,
  ENABLE_ADBLOCK : 1,
  ENABLE_SMART_ADBLOCK : 2,
}

var PopupBlockerSettings = {
  DISABLE_POPUP_BLOCKER : 0,
  BLOCK_WINDOW_POPUP : 1,
  BLOCK_ALL_POPUP : 2,
}

cr.define('adblock_settings', function() {
  'use strict';

  function initialize() {
    // Adblock
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
    // Popup blocker
    var popupBlockerSetting = loadTimeData.getInteger('popupBlockerSetting');
    if (popupBlockerSetting == PopupBlockerSettings.DISABLE_POPUP_BLOCKER) {
      $('disable_popup_blocker').checked = true;
    }
    else if (popupBlockerSetting == PopupBlockerSettings.BLOCK_WINDOW_POPUP) {
      $('block_window_popup').checked = true;
    }
    else if (popupBlockerSetting == PopupBlockerSettings.BLOCK_ALL_POPUP){
      $('block_all_popup').checked = true;
    }
    $('disable_popup_blocker').onclick = function() {
      adblock_settings.settingPopupBlocker(PopupBlockerSettings.DISABLE_POPUP_BLOCKER);
    };
    $('block_window_popup').onclick = function() {
      adblock_settings.settingPopupBlocker(PopupBlockerSettings.BLOCK_WINDOW_POPUP);
    };
    $('block_all_popup').onclick = function() {
      adblock_settings.settingPopupBlocker(PopupBlockerSettings.BLOCK_ALL_POPUP);
    };
  }

  function settingAdblock(value) {
    chrome.send("settingAdblock", [value]);
  }

  function settingPopupBlocker(value) {
    chrome.send("settingPopupBlocker", [value]);
  }

  // Return an object with all of the exports.
  return {
    initialize: initialize,
    settingAdblock: settingAdblock,
    settingPopupBlocker, settingPopupBlocker
  };
});

document.addEventListener('DOMContentLoaded', adblock_settings.initialize);
