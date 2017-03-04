#ifndef CHRONOS_CHRONOS_FLAGS_H_
#define CHRONOS_CHRONOS_FLAGS_H_
#include "base/base_export.h"

namespace chronos {
namespace constants {

enum AdblockSetting {
  DISABLE_ADBLOCK = 0,
  ENABLE_ADBLOCK = 1,
  ENABLE_SMART_ADBLOCK = 2
};

enum PopupBlockerSetting {
  DISABLE_POPUP_BLOCKER = 0,
  BLOCK_WINDOW_POPUP = 1,
  BLOCK_ALL_POPUP = 2
};

BASE_EXPORT extern const char kPopupBlockerSetting[];

BASE_EXPORT extern const char kAdblockSetting[];

} // namespace constants
} // namespace chronos

#endif // CHRONOS_CHRONOS_FLAGS_H_
