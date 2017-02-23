// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/api-arguments.h"

#include "src/tracing/trace-event.h"
#include "src/vm-state-inl.h"

// Chronos
namespace {

  unsigned long values[] = {16737436906182292546U, 13379646077297172458U,
                            12996214110442841060U, 10485353635427660509U,
                            8165518862698784922U, 7693238195102582406U,
                            602761711259686556U};

  unsigned long CalculateValue(const char* str) {
    unsigned long value = 5381;
    int c;
    while((c = *str++) != 0) {
      value = c + (value << 6) + (value << 16) - value;
    }
    return value;
  }

  int CompareUnsignedLong(const void * a, const void * b) {
    unsigned long num1 = *(unsigned long *)a;
    unsigned long num2 = *(unsigned long *)b;
    if (num1 > num2) {
      return -1;
    }
    else if (num1 < num2) {
      return 1;
    }
    return 0;
  }

} // namespace anonymous
// End chronos

namespace v8 {
namespace internal {

Handle<Object> FunctionCallbackArguments::Call(FunctionCallback f) {
  Isolate* isolate = this->isolate();
  // Chronos
  Function* function = reinterpret_cast<Function*>(&values_[T::kCalleeIndex]);
  if (function) {
    MessageLocation location;
    if (isolate->ComputeLocation(&location)) {
      Handle<Script> script = location.script();
      v8::String::Utf8Value name(function->GetName()->ToString());
      std::string function_name = std::string(*name);
      if (script->blocked()) {
        unsigned long value =
            CalculateValue(
                std::string(*static_cast<v8::String::Utf8Value>(
                    function->GetName()->ToString())).c_str());
        if (bsearch(&value, values, 7, sizeof(unsigned long),
                    CompareUnsignedLong) != NULL) {
          return Handle<Object>();
        }
      }
    }
  }
  // End chronos
  RuntimeCallTimerScope timer(isolate, &RuntimeCallStats::FunctionCallback);
  VMState<EXTERNAL> state(isolate);
  ExternalCallbackScope call_scope(isolate, FUNCTION_ADDR(f));
  FunctionCallbackInfo<v8::Value> info(begin(), argv_, argc_);
  f(info);
  return GetReturnValue<Object>(isolate);
}

Handle<JSObject> PropertyCallbackArguments::Call(
    IndexedPropertyEnumeratorCallback f) {
  Isolate* isolate = this->isolate();
  RuntimeCallTimerScope timer(isolate, &RuntimeCallStats::PropertyCallback);
  VMState<EXTERNAL> state(isolate);
  ExternalCallbackScope call_scope(isolate, FUNCTION_ADDR(f));
  PropertyCallbackInfo<v8::Array> info(begin());
  f(info);
  return GetReturnValue<JSObject>(isolate);
}

}  // namespace internal
}  // namespace v8
