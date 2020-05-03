// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Modified 2001 by Colt McAnlis (colton@google.com)

// This header is designed to give you trace_event macros without specifying
// how the events actually get collected and stored. If you need to expose trace
// event to some other universe, you can copy-and-paste this file,
// implement the TRACE_EVENT_API macros, and do any other necessary fixup for
// the target platform. The end result is that multiple libraries can funnel
// events through to a shared trace event collector.

#ifndef BASE_DEBUG_TRACE_EVENT_H_
#define BASE_DEBUG_TRACE_EVENT_H_
#pragma once

#include <string>
#include "event_trace_plat.h"
//#include "base/atomicops.h"
#include "event_trace_impl.h"
//#include "build/build_config.h"

// By default, const char* argument values are assumed to have long-lived scope
// and will not be copied. Use this macro to force a const char* to be copied.
#define TRACE_STR_COPY(str) event_trace::TraceStringWithCopy(str)

// Records a pair of begin and end events called "name" for the current
// scope, with 0, 1 or 2 associated arguments. If the category is not
// enabled, then this does nothing.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
#define TRACE_EVENT0(category, name) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED(category, name)
#define TRACE_EVENT1(category, name, arg1_name, arg1_val) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED(category, name, arg1_name, arg1_val)
#define TRACE_EVENT2(category, name, arg1_name, arg1_val, arg2_name, arg2_val) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED(category, name, arg1_name, arg1_val,         \
                                  arg2_name, arg2_val)

// Same as TRACE_EVENT except that they are not included in official builds.
#ifdef OFFICIAL_BUILD
#define UNSHIPPED_TRACE_EVENT0(category, name) (void)0
#define UNSHIPPED_TRACE_EVENT1(category, name, arg1_name, arg1_val) (void)0
#define UNSHIPPED_TRACE_EVENT2(category, name, arg1_name, arg1_val, arg2_name, \
                               arg2_val)                                       \
  (void)0
#define UNSHIPPED_TRACE_EVENT_INSTANT0(category, name) (void)0
#define UNSHIPPED_TRACE_EVENT_INSTANT1(category, name, arg1_name, arg1_val) \
  (void)0
#define UNSHIPPED_TRACE_EVENT_INSTANT2(category, name, arg1_name, arg1_val, \
                                       arg2_name, arg2_val)                 \
  (void)0
#else
#define UNSHIPPED_TRACE_EVENT0(category, name) TRACE_EVENT0(category, name)
#define UNSHIPPED_TRACE_EVENT1(category, name, arg1_name, arg1_val) \
  TRACE_EVENT1(category, name, arg1_name, arg1_val)
#define UNSHIPPED_TRACE_EVENT2(category, name, arg1_name, arg1_val, arg2_name, \
                               arg2_val)                                       \
  TRACE_EVENT2(category, name, arg1_name, arg1_val, arg2_name, arg2_val)
#define UNSHIPPED_TRACE_EVENT_INSTANT0(category, name) \
  TRACE_EVENT_INSTANT0(category, name)
#define UNSHIPPED_TRACE_EVENT_INSTANT1(category, name, arg1_name, arg1_val) \
  TRACE_EVENT_INSTANT1(category, name, arg1_name, arg1_val)
#define UNSHIPPED_TRACE_EVENT_INSTANT2(category, name, arg1_name, arg1_val, \
                                       arg2_name, arg2_val)                 \
  TRACE_EVENT_INSTANT2(category, name, arg1_name, arg1_val, arg2_name, arg2_val)
#endif

#define TRACE_EVENT_BEGIN() \
  event_trace::TraceLog::GetInstance()->BeginLogging();
#define TRACE_EVENT_FINISH() \
  TRACE_EVENT_FLUSH_LOG();   \
  event_trace::TraceLog::GetInstance()->EndLogging();
#define TRACE_EVENT_FLUSH_LOG() event_trace::TraceLog::GetInstance()->Flush();

// Records a single event called "name" immediately, with 0, 1 or 2
// associated arguments. If the category is not enabled, then this
// does nothing.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
#define TRACE_EVENT_INSTANT0(category, name)                          \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category, name, \
                           TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_INSTANT1(category, name, arg1_name, arg1_val)     \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category, name, \
                           TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_INSTANT2(category, name, arg1_name, arg1_val, arg2_name, \
                             arg2_val)                                       \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category, name,        \
                           TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val,       \
                           arg2_name, arg2_val)
#define TRACE_EVENT_COPY_INSTANT0(category, name)                     \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category, name, \
                           TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_INSTANT1(category, name, arg1_name, arg1_val) \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category, name,  \
                           TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val)
#define TRACE_EVENT_COPY_INSTANT2(category, name, arg1_name, arg1_val, \
                                  arg2_name, arg2_val)                 \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, category, name,  \
                           TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val, \
                           arg2_name, arg2_val)

// Records a single BEGIN event called "name" immediately, with 0, 1 or 2
// associated arguments. If the category is not enabled, then this
// does nothing.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
#define TRACE_EVENT_BEGIN0(category, name)                          \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, category, name, \
                           TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_BEGIN1(category, name, arg1_name, arg1_val)     \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, category, name, \
                           TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_BEGIN2(category, name, arg1_name, arg1_val, arg2_name, \
                           arg2_val)                                       \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, category, name,        \
                           TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val,     \
                           arg2_name, arg2_val)
#define TRACE_EVENT_COPY_BEGIN0(category, name)                     \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, category, name, \
                           TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_BEGIN1(category, name, arg1_name, arg1_val) \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, category, name,  \
                           TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val)
#define TRACE_EVENT_COPY_BEGIN2(category, name, arg1_name, arg1_val,   \
                                arg2_name, arg2_val)                   \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, category, name,    \
                           TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val, \
                           arg2_name, arg2_val)

// Records a single END event for "name" immediately. If the category
// is not enabled, then this does nothing.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
#define TRACE_EVENT_END0(category, name)                          \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, category, name, \
                           TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_END1(category, name, arg1_name, arg1_val)     \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, category, name, \
                           TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_END2(category, name, arg1_name, arg1_val, arg2_name, \
                         arg2_val)                                       \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, category, name,        \
                           TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val,   \
                           arg2_name, arg2_val)
#define TRACE_EVENT_COPY_END0(category, name)                     \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, category, name, \
                           TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_END1(category, name, arg1_name, arg1_val) \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, category, name,  \
                           TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val)
#define TRACE_EVENT_COPY_END2(category, name, arg1_name, arg1_val, arg2_name, \
                              arg2_val)                                       \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, category, name,             \
                           TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val,        \
                           arg2_name, arg2_val)

// Time threshold event:
// Only record the event if the duration is greater than the specified
// threshold_us (time in microseconds).
// Records a pair of begin and end events called "name" for the current
// scope, with 0, 1 or 2 associated arguments. If the category is not
// enabled, then this does nothing.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
#define TRACE_EVENT_IF_LONGER_THAN0(threshold_us, category, name) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED_IF_LONGER_THAN(threshold_us, category, name)
#define TRACE_EVENT_IF_LONGER_THAN1(threshold_us, category, name, arg1_name,   \
                                    arg1_val)                                  \
  INTERNAL_TRACE_EVENT_ADD_SCOPED_IF_LONGER_THAN(threshold_us, category, name, \
                                                 arg1_name, arg1_val)
#define TRACE_EVENT_IF_LONGER_THAN2(threshold_us, category, name, arg1_name, \
                                    arg1_val, arg2_name, arg2_val)           \
  INTERNAL_TRACE_EVENT_ADD_SCOPED_IF_LONGER_THAN(                            \
      threshold_us, category, name, arg1_name, arg1_val, arg2_name, arg2_val)

// Records the value of a counter called "name" immediately. Value
// must be representable as a 32 bit integer.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
#define TRACE_COUNTER1(category, name, value)                         \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, category, name, \
                           TRACE_EVENT_FLAG_NONE, "value",            \
                           static_cast<int>(value))
#define TRACE_COPY_COUNTER1(category, name, value)                    \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, category, name, \
                           TRACE_EVENT_FLAG_COPY, "value",            \
                           static_cast<int>(value))

// Records the values of a multi-parted counter called "name" immediately.
// The UI will treat value1 and value2 as parts of a whole, displaying their
// values as a stacked-bar chart.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
#define TRACE_COUNTER2(category, name, value1_name, value1_val, value2_name, \
                       value2_val)                                           \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, category, name,        \
                           TRACE_EVENT_FLAG_NONE, value1_name,               \
                           static_cast<int>(value1_val), value2_name,        \
                           static_cast<int>(value2_val))
#define TRACE_COPY_COUNTER2(category, name, value1_name, value1_val,  \
                            value2_name, value2_val)                  \
  INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, category, name, \
                           TRACE_EVENT_FLAG_COPY, value1_name,        \
                           static_cast<int>(value1_val), value2_name, \
                           static_cast<int>(value2_val))

// Records the value of a counter called "name" immediately. Value
// must be representable as a 32 bit integer.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
// - |id| is used to disambiguate counters with the same name. It must either
//   be a pointer or an integer value up to 64 bits. If it's a pointer, the bits
//   will be xored with a hash of the process ID so that the same pointer on
//   two different processes will not collide.
#define TRACE_COUNTER_ID1(category, name, id, value)                          \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_COUNTER, category, name, \
                                   id, TRACE_EVENT_FLAG_NONE, "value",        \
                                   static_cast<int>(value))
#define TRACE_COPY_COUNTER_ID1(category, name, id, value)                     \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_COUNTER, category, name, \
                                   id, TRACE_EVENT_FLAG_COPY, "value",        \
                                   static_cast<int>(value))

// Records the values of a multi-parted counter called "name" immediately.
// The UI will treat value1 and value2 as parts of a whole, displaying their
// values as a stacked-bar chart.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
// - |id| is used to disambiguate counters with the same name. It must either
//   be a pointer or an integer value up to 64 bits. If it's a pointer, the bits
//   will be xored with a hash of the process ID so that the same pointer on
//   two different processes will not collide.
#define TRACE_COUNTER_ID2(category, name, id, value1_name, value1_val,        \
                          value2_name, value2_val)                            \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_COUNTER, category, name, \
                                   id, TRACE_EVENT_FLAG_NONE, value1_name,    \
                                   static_cast<int>(value1_val), value2_name, \
                                   static_cast<int>(value2_val))
#define TRACE_COPY_COUNTER_ID2(category, name, id, value1_name, value1_val,   \
                               value2_name, value2_val)                       \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_COUNTER, category, name, \
                                   id, TRACE_EVENT_FLAG_COPY, value1_name,    \
                                   static_cast<int>(value1_val), value2_name, \
                                   static_cast<int>(value2_val))

// Records a single ASYNC_BEGIN event called "name" immediately, with 0, 1 or 2
// associated arguments. If the category is not enabled, then this
// does nothing.
// - category and name strings must have application lifetime (statics or
//   literals). They may not include " chars.
// - |id| is used to match the ASYNC_BEGIN event with the ASYNC_END event. ASYNC
//   events are considered to match if their category, name and id values all
//   match. |id| must either be a pointer or an integer value up to 64 bits. If
//   it's a pointer, the bits will be xored with a hash of the process ID so
//   that the same pointer on two different processes will not collide.
// An asynchronous operation can consist of multiple phases. The first phase is
// defined by the ASYNC_BEGIN calls. Additional phases can be defined using the
// ASYNC_STEP_BEGIN macros. When the operation completes, call ASYNC_END.
// An async operation can span threads and processes, but all events in that
// operation must use the same |name| and |id|. Each event can have its own
// args.
#define TRACE_EVENT_ASYNC_BEGIN0(category, name, id)                        \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, category, \
                                   name, id, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_ASYNC_BEGIN1(category, name, id, arg1_name, arg1_val)      \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, category,    \
                                   name, id, TRACE_EVENT_FLAG_NONE, arg1_name, \
                                   arg1_val)
#define TRACE_EVENT_ASYNC_BEGIN2(category, name, id, arg1_name, arg1_val,      \
                                 arg2_name, arg2_val)                          \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, category,    \
                                   name, id, TRACE_EVENT_FLAG_NONE, arg1_name, \
                                   arg1_val, arg2_name, arg2_val)
#define TRACE_EVENT_COPY_ASYNC_BEGIN0(category, name, id)                   \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, category, \
                                   name, id, TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_ASYNC_BEGIN1(category, name, id, arg1_name, arg1_val) \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, category,    \
                                   name, id, TRACE_EVENT_FLAG_COPY, arg1_name, \
                                   arg1_val)
#define TRACE_EVENT_COPY_ASYNC_BEGIN2(category, name, id, arg1_name, arg1_val, \
                                      arg2_name, arg2_val)                     \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, category,    \
                                   name, id, TRACE_EVENT_FLAG_COPY, arg1_name, \
                                   arg1_val, arg2_name, arg2_val)

// Records a single ASYNC_STEP event for |step| immediately. If the category
// is not enabled, then this does nothing. The |name| and |id| must match the
// ASYNC_BEGIN event above. The |step| param identifies this step within the
// async event. This should be called at the beginning of the next phase of an
// asynchronous operation.
#define TRACE_EVENT_ASYNC_BEGIN_STEP0(category, name, id, step)             \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_STEP, category,  \
                                   name, id, TRACE_EVENT_FLAG_NONE, "step", \
                                   step)
#define TRACE_EVENT_ASYNC_BEGIN_STEP1(category, name, id, step, arg1_name,  \
                                      arg1_val)                             \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_STEP, category,  \
                                   name, id, TRACE_EVENT_FLAG_NONE, "step", \
                                   step, arg1_name, arg1_val)
#define TRACE_EVENT_COPY_ASYNC_BEGIN_STEP0(category, name, id, step)        \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_STEP, category,  \
                                   name, id, TRACE_EVENT_FLAG_COPY, "step", \
                                   step)
#define TRACE_EVENT_COPY_ASYNC_BEGIN_STEP1(category, name, id, step,        \
                                           arg1_name, arg1_val)             \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_STEP, category,  \
                                   name, id, TRACE_EVENT_FLAG_COPY, "step", \
                                   step, arg1_name, arg1_val)

// Records a single ASYNC_END event for "name" immediately. If the category
// is not enabled, then this does nothing.
#define TRACE_EVENT_ASYNC_END0(category, name, id)                        \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, category, \
                                   name, id, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_ASYNC_END1(category, name, id, arg1_name, arg1_val)        \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, category,      \
                                   name, id, TRACE_EVENT_FLAG_NONE, arg1_name, \
                                   arg1_val)
#define TRACE_EVENT_ASYNC_END2(category, name, id, arg1_name, arg1_val,        \
                               arg2_name, arg2_val)                            \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, category,      \
                                   name, id, TRACE_EVENT_FLAG_NONE, arg1_name, \
                                   arg1_val, arg2_name, arg2_val)
#define TRACE_EVENT_COPY_ASYNC_END0(category, name, id)                   \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, category, \
                                   name, id, TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_ASYNC_END1(category, name, id, arg1_name, arg1_val)   \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, category,      \
                                   name, id, TRACE_EVENT_FLAG_COPY, arg1_name, \
                                   arg1_val)
#define TRACE_EVENT_COPY_ASYNC_END2(category, name, id, arg1_name, arg1_val,   \
                                    arg2_name, arg2_val)                       \
  INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, category,      \
                                   name, id, TRACE_EVENT_FLAG_COPY, arg1_name, \
                                   arg1_val, arg2_name, arg2_val)

////////////////////////////////////////////////////////////////////////////////
// Implementation specific tracing API definitions.

// Get a pointer to the enabled state of the given trace category. Only
// long-lived literal strings should be given as the category name. The returned
// pointer can be held permanently in a local static for example. If the
// unsigned char is non-zero, tracing is enabled. If tracing is enabled,
// TRACE_EVENT_API_ADD_TRACE_EVENT can be called. It's OK if tracing is disabled
// between the load of the tracing state and the call to
// TRACE_EVENT_API_ADD_TRACE_EVENT, because this flag only provides an early out
// for best performance when tracing is disabled.
// const unsigned char*
//     TRACE_EVENT_API_GET_CATEGORY_ENABLED(const char* category_name)
#define TRACE_EVENT_API_GET_CATEGORY_ENABLED \
  event_trace::TraceLog::GetCategoryEnabled

// Add a trace event to the platform tracing system. Returns thresholdBeginId
// for use in a corresponding end TRACE_EVENT_API_ADD_TRACE_EVENT call.
// int TRACE_EVENT_API_ADD_TRACE_EVENT(
//                    char phase,
//                    const unsigned char* category_enabled,
//                    const char* name,
//                    unsigned long long id,
//                    int num_args,
//                    const char** arg_names,
//                    const unsigned char* arg_types,
//                    const unsigned long long* arg_values,
//                    int threshold_begin_id,
//                    long long threshold,
//                    unsigned char flags)
#define TRACE_EVENT_API_ADD_TRACE_EVENT \
  event_trace::TraceLog::GetInstance()->AddTraceEvent

////////////////////////////////////////////////////////////////////////////////

// Implementation detail: trace event macros create temporary variables
// to keep instrumentation overhead low. These macros give each temporary
// variable a unique name based on the line number to prevent name collissions.
#define INTERNAL_TRACE_EVENT_UID3(a, b) trace_event_unique_##a##b
#define INTERNAL_TRACE_EVENT_UID2(a, b) INTERNAL_TRACE_EVENT_UID3(a, b)
#define INTERNAL_TRACE_EVENT_UID(name_prefix) \
  INTERNAL_TRACE_EVENT_UID2(name_prefix, __LINE__)

// Implementation detail: internal macro to create static category.
// No barriers are needed, because this code is designed to operate safely
// even when the unsigned char* points to garbage data (which may be the case
// on processors without cache coherency).

// CLM NOTE - I've removed the thread safety here.. which is most likely a bad
// thing.. but OK for my testing...
#define NoBarrier_Load(var) var
#define NoBarrier_Store(var, val) var = val;
#define AtomicWord const event_trace::uint16*

#define INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category)                    \
  static AtomicWord INTERNAL_TRACE_EVENT_UID(atomic) = 0;                   \
  const event_trace::uint8* INTERNAL_TRACE_EVENT_UID(catstatic) =           \
      reinterpret_cast<const event_trace::uint8*>(                          \
          NoBarrier_Load(INTERNAL_TRACE_EVENT_UID(atomic)));                \
  if (!INTERNAL_TRACE_EVENT_UID(catstatic)) {                               \
    INTERNAL_TRACE_EVENT_UID(catstatic) =                                   \
        TRACE_EVENT_API_GET_CATEGORY_ENABLED(category);                     \
    NoBarrier_Store(                                                        \
        INTERNAL_TRACE_EVENT_UID(atomic),                                   \
        reinterpret_cast<AtomicWord>(INTERNAL_TRACE_EVENT_UID(catstatic))); \
  }

// Implementation detail: internal macro to create static category and add
// event if the category is enabled.
#define INTERNAL_TRACE_EVENT_ADD(phase, category, name, flags, ...)          \
  do {                                                                       \
    INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category);                        \
    if (*INTERNAL_TRACE_EVENT_UID(catstatic)) {                              \
      event_trace::AddTraceEvent(phase, INTERNAL_TRACE_EVENT_UID(catstatic), \
                                 name, event_trace::kNoEventId, flags,       \
                                 ##__VA_ARGS__);                             \
    }                                                                        \
  } while (0)

// Implementation detail: internal macro to create static category and add begin
// event if the category is enabled. Also adds the end event when the scope
// ends.
#define INTERNAL_TRACE_EVENT_ADD_SCOPED(category, name, ...)                \
  INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category);                         \
  event_trace::TraceEndOnScopeClose INTERNAL_TRACE_EVENT_UID(profileScope); \
  if (*INTERNAL_TRACE_EVENT_UID(catstatic)) {                               \
    event_trace::AddTraceEvent(                                             \
        TRACE_EVENT_PHASE_BEGIN, INTERNAL_TRACE_EVENT_UID(catstatic), name, \
        event_trace::kNoEventId, TRACE_EVENT_FLAG_NONE, ##__VA_ARGS__);     \
    INTERNAL_TRACE_EVENT_UID(profileScope)                                  \
        .Initialize(INTERNAL_TRACE_EVENT_UID(catstatic), name);             \
  }

// Implementation detail: internal macro to create static category and add begin
// event if the category is enabled. Also adds the end event when the scope
// ends. If the elapsed time is < threshold time, the begin/end pair is erased.
#define INTERNAL_TRACE_EVENT_ADD_SCOPED_IF_LONGER_THAN(threshold, category,    \
                                                       name, ...)              \
  INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category);                            \
  event_trace::TraceEndOnScopeCloseThreshold INTERNAL_TRACE_EVENT_UID(         \
      profileScope);                                                           \
  if (*INTERNAL_TRACE_EVENT_UID(catstatic)) {                                  \
    int INTERNAL_TRACE_EVENT_UID(begin_event_id) = event_trace::AddTraceEvent( \
        TRACE_EVENT_PHASE_BEGIN, INTERNAL_TRACE_EVENT_UID(catstatic), name,    \
        event_trace::kNoEventId, TRACE_EVENT_FLAG_NONE, ##__VA_ARGS__);        \
    INTERNAL_TRACE_EVENT_UID(profileScope)                                     \
        .Initialize(INTERNAL_TRACE_EVENT_UID(catstatic), name,                 \
                    INTERNAL_TRACE_EVENT_UID(begin_event_id), threshold);      \
  }

// Implementation detail: internal macro to create static category and add
// event if the category is enabled.
#define INTERNAL_TRACE_EVENT_ADD_WITH_ID(phase, category, name, id, flags,   \
                                         ...)                                \
  do {                                                                       \
    INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category);                        \
    if (*INTERNAL_TRACE_EVENT_UID(catstatic)) {                              \
      unsigned char trace_event_flags = flags | TRACE_EVENT_FLAG_HAS_ID;     \
      event_trace::TraceID trace_event_trace_id(id, &trace_event_flags);     \
      event_trace::AddTraceEvent(phase, INTERNAL_TRACE_EVENT_UID(catstatic), \
                                 name, trace_event_trace_id.data(),          \
                                 trace_event_flags, ##__VA_ARGS__);          \
    }                                                                        \
  } while (0)

// Notes regarding the following definitions:
// New values can be added and propagated to third party libraries, but existing
// definitions must never be changed, because third party libraries may use old
// definitions.

// Phase indicates the nature of an event entry. E.g. part of a begin/end pair.
#define TRACE_EVENT_PHASE_BEGIN ('B')
#define TRACE_EVENT_PHASE_END ('E')
#define TRACE_EVENT_PHASE_INSTANT ('I')
#define TRACE_EVENT_PHASE_ASYNC_BEGIN ('S')
#define TRACE_EVENT_PHASE_ASYNC_STEP ('T')
#define TRACE_EVENT_PHASE_ASYNC_END ('F')
#define TRACE_EVENT_PHASE_METADATA ('M')
#define TRACE_EVENT_PHASE_COUNTER ('C')

// Flags for changing the behavior of TRACE_EVENT_API_ADD_TRACE_EVENT.
#define TRACE_EVENT_FLAG_NONE (static_cast<unsigned char>(0))
#define TRACE_EVENT_FLAG_COPY (static_cast<unsigned char>(1 << 0))
#define TRACE_EVENT_FLAG_HAS_ID (static_cast<unsigned char>(1 << 1))
#define TRACE_EVENT_FLAG_MANGLE_ID (static_cast<unsigned char>(1 << 2))

// Type values for identifying types in the TraceValue union.
#define TRACE_VALUE_TYPE_BOOL (static_cast<unsigned char>(1))
#define TRACE_VALUE_TYPE_UINT (static_cast<unsigned char>(2))
#define TRACE_VALUE_TYPE_INT (static_cast<unsigned char>(3))
#define TRACE_VALUE_TYPE_DOUBLE (static_cast<unsigned char>(4))
#define TRACE_VALUE_TYPE_POINTER (static_cast<unsigned char>(5))
#define TRACE_VALUE_TYPE_STRING (static_cast<unsigned char>(6))
#define TRACE_VALUE_TYPE_COPY_STRING (static_cast<unsigned char>(7))

namespace event_trace {

// Specify these values when the corresponding argument of AddTraceEvent is not
// used.
const int kZeroNumArgs = 0;
const int kNoThreshholdBeginId = -1;
const long long kNoThresholdValue = 0;
const unsigned long long kNoEventId = 0;

// TraceID encapsulates an ID that can either be an integer or pointer. Pointers
// are mangled with the Process ID so that they are unlikely to collide when the
// same pointer is used on different processes.
class TraceID {
 public:
  explicit TraceID(const void* id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(
            reinterpret_cast<unsigned long>(id))) {
    *flags |= TRACE_EVENT_FLAG_MANGLE_ID;
  }
  explicit TraceID(unsigned long long id, unsigned char* flags) : data_(id) {
    (void)flags;
  }
  explicit TraceID(unsigned long id, unsigned char* flags) : data_(id) {
    (void)flags;
  }
  explicit TraceID(unsigned int id, unsigned char* flags) : data_(id) {
    (void)flags;
  }
  explicit TraceID(unsigned short id, unsigned char* flags) : data_(id) {
    (void)flags;
  }
  explicit TraceID(unsigned char id, unsigned char* flags) : data_(id) {
    (void)flags;
  }
  explicit TraceID(long long id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) {
    (void)flags;
  }
  explicit TraceID(long id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) {
    (void)flags;
  }
  explicit TraceID(int id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) {
    (void)flags;
  }
  explicit TraceID(short id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) {
    (void)flags;
  }
  explicit TraceID(signed char id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) {
    (void)flags;
  }

  unsigned long long data() const { return data_; }

 private:
  unsigned long long data_;
};

// Simple union to store various types as unsigned long long.
union TraceValueUnion {
  bool as_bool;
  unsigned long long as_uint;
  long long as_int;
  double as_double;
  const void* as_pointer;
  const char* as_string;
};

// Simple container for const char* that should be copied instead of retained.
class TraceStringWithCopy {
 public:
  explicit TraceStringWithCopy(const char* str) : str_(str) {}
  operator const char*() const { return str_; }

 private:
  const char* str_;
};

// Define SetTraceValue for each allowed type. It stores the type and
// value in the return arguments. This allows this API to avoid declaring any
// structures so that it is portable to third_party libraries.
#define INTERNAL_DECLARE_SET_TRACE_VALUE(actual_type, union_member,      \
                                         value_type_id)                  \
  static inline void SetTraceValue(actual_type arg, unsigned char* type, \
                                   unsigned long long* value) {          \
    TraceValueUnion type_value;                                          \
    type_value.union_member = arg;                                       \
    *type = value_type_id;                                               \
    *value = type_value.as_uint;                                         \
  }
// Simpler form for int types that can be safely casted.
#define INTERNAL_DECLARE_SET_TRACE_VALUE_INT(actual_type, value_type_id) \
  static inline void SetTraceValue(actual_type arg, unsigned char* type, \
                                   unsigned long long* value) {          \
    *type = value_type_id;                                               \
    *value = static_cast<unsigned long long>(arg);                       \
  }

INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned long long, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned int, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned short, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned char, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(long long, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(int, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(short, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(signed char, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE(bool, as_bool, TRACE_VALUE_TYPE_BOOL)
INTERNAL_DECLARE_SET_TRACE_VALUE(double, as_double, TRACE_VALUE_TYPE_DOUBLE)
INTERNAL_DECLARE_SET_TRACE_VALUE(const void*,
                                 as_pointer,
                                 TRACE_VALUE_TYPE_POINTER)
INTERNAL_DECLARE_SET_TRACE_VALUE(const char*,
                                 as_string,
                                 TRACE_VALUE_TYPE_STRING)
INTERNAL_DECLARE_SET_TRACE_VALUE(const TraceStringWithCopy&,
                                 as_string,
                                 TRACE_VALUE_TYPE_COPY_STRING)

#undef INTERNAL_DECLARE_SET_TRACE_VALUE
#undef INTERNAL_DECLARE_SET_TRACE_VALUE_INT

// std::string version of SetTraceValue so that trace arguments can be strings.
static inline void SetTraceValue(const std::string& arg,
                                 unsigned char* type,
                                 unsigned long long* value) {
  TraceValueUnion type_value;
  type_value.as_string = arg.c_str();
  *type = TRACE_VALUE_TYPE_COPY_STRING;
  *value = type_value.as_uint;
}

// These AddTraceEvent template functions are defined here instead of in the
// macro, because the arg_values could be temporary objects, such as
// std::string. In order to store pointers to the internal c_str and pass
// through to the tracing API, the arg_values must live throughout
// these procedures.

static inline int AddTraceEvent(char phase,
                                const unsigned char* category_enabled,
                                const char* name,
                                unsigned long long id,
                                unsigned char flags) {
  return TRACE_EVENT_API_ADD_TRACE_EVENT(
      phase, category_enabled, name, id, kZeroNumArgs, NULL, NULL, NULL,
      kNoThreshholdBeginId, kNoThresholdValue, flags);
}

template <class ARG1_TYPE>
static inline int AddTraceEvent(char phase,
                                const unsigned char* category_enabled,
                                const char* name,
                                unsigned long long id,
                                unsigned char flags,
                                const char* arg1_name,
                                const ARG1_TYPE& arg1_val) {
  const int num_args = 1;
  unsigned char arg_types[1];
  unsigned long long arg_values[1];
  SetTraceValue(arg1_val, &arg_types[0], &arg_values[0]);
  return TRACE_EVENT_API_ADD_TRACE_EVENT(
      phase, category_enabled, name, id, num_args, &arg1_name, arg_types,
      arg_values, kNoThreshholdBeginId, kNoThresholdValue, flags);
}

template <class ARG1_TYPE, class ARG2_TYPE>
static inline int AddTraceEvent(char phase,
                                const unsigned char* category_enabled,
                                const char* name,
                                unsigned long long id,
                                unsigned char flags,
                                const char* arg1_name,
                                const ARG1_TYPE& arg1_val,
                                const char* arg2_name,
                                const ARG2_TYPE& arg2_val) {
  const int num_args = 2;
  const char* arg_names[2] = {arg1_name, arg2_name};
  unsigned char arg_types[2];
  unsigned long long arg_values[2];
  SetTraceValue(arg1_val, &arg_types[0], &arg_values[0]);
  SetTraceValue(arg2_val, &arg_types[1], &arg_values[1]);
  return TRACE_EVENT_API_ADD_TRACE_EVENT(
      phase, category_enabled, name, id, num_args, arg_names, arg_types,
      arg_values, kNoThreshholdBeginId, kNoThresholdValue, flags);
}

// Used by TRACE_EVENTx macro. Do not use directly.
class TraceEndOnScopeClose {
 public:
  // Note: members of data_ intentionally left uninitialized. See Initialize.
  TraceEndOnScopeClose() : p_data_(NULL) {}
  ~TraceEndOnScopeClose() {
    if (p_data_)
      AddEventIfEnabled();
  }

  void Initialize(const unsigned char* category_enabled, const char* name);

 private:
  // Add the end event if the category is still enabled.
  void AddEventIfEnabled();

  // This Data struct workaround is to avoid initializing all the members
  // in Data during construction of this object, since this object is always
  // constructed, even when tracing is disabled. If the members of Data were
  // members of this class instead, compiler warnings occur about potential
  // uninitialized accesses.
  struct Data {
    const unsigned char* category_enabled;
    const char* name;
  };
  Data* p_data_;
  Data data_;
};

// Used by TRACE_EVENTx macro. Do not use directly.
class TraceEndOnScopeCloseThreshold {
 public:
  // Note: members of data_ intentionally left uninitialized. See Initialize.
  TraceEndOnScopeCloseThreshold() : p_data_(NULL) {}
  ~TraceEndOnScopeCloseThreshold() {
    if (p_data_)
      AddEventIfEnabled();
  }

  // Called by macros only when tracing is enabled at the point when the begin
  // event is added.
  void Initialize(const unsigned char* category_enabled,
                  const char* name,
                  int threshold_begin_id,
                  long long threshold);

 private:
  // Add the end event if the category is still enabled.
  void AddEventIfEnabled();

  // This Data struct workaround is to avoid initializing all the members
  // in Data during construction of this object, since this object is always
  // constructed, even when tracing is disabled. If the members of Data were
  // members of this class instead, compiler warnings occur about potential
  // uninitialized accesses.
  struct Data {
    long long threshold;
    const unsigned char* category_enabled;
    const char* name;
    int threshold_begin_id;
  };
  Data* p_data_;
  Data data_;
};

}  // namespace event_trace

#endif  // BASE_DEBUG_TRACE_EVENT_H_