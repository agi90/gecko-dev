/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Assertions.h"
#include "mozilla/Atomics.h"

#include <stdarg.h>

MOZ_BEGIN_EXTERN_C

/*
 * The crash reason is defined as a global variable here rather than in the
 * crash reporter itself to make it available to all code, even libraries like
 * JS that don't link with the crash reporter directly. This value will only
 * be consumed if the crash reporter is used by the target application.
 */
MFBT_DATA const char* gMozCrashReason = nullptr;

static char sPrintfCrashReason[sPrintfCrashReasonSize] = {};

// Accesses to this atomic are not included in web replay recordings, so that
// if we crash in an area where recorded events are not allowed the true reason
// for the crash is not obscured by a record/replay error.
static mozilla::Atomic<bool, mozilla::SequentiallyConsistent,
                       mozilla::recordreplay::Behavior::DontPreserve> sCrashing(false);

#ifndef DEBUG
MFBT_API MOZ_COLD MOZ_NORETURN MOZ_NEVER_INLINE MOZ_FORMAT_PRINTF(2, 3) void
MOZ_CrashPrintf(int aLine, const char* aFormat, ...)
#else
MFBT_API MOZ_COLD MOZ_NORETURN MOZ_NEVER_INLINE MOZ_FORMAT_PRINTF(3, 4) void
MOZ_CrashPrintf(const char* aFilename, int aLine, const char* aFormat, ...)
#endif
{
  if (!sCrashing.compareExchange(false, true)) {
    // In the unlikely event of a race condition, skip
    // setting the crash reason and just crash safely.
    MOZ_REALLY_CRASH(aLine);
  }
  va_list aArgs;
  va_start(aArgs, aFormat);
  int ret = vsnprintf(sPrintfCrashReason, sPrintfCrashReasonSize,
                      aFormat, aArgs);
  va_end(aArgs);
  MOZ_RELEASE_ASSERT(ret >= 0 && size_t(ret) < sPrintfCrashReasonSize,
    "Could not write the explanation string to the supplied buffer!");
#ifdef DEBUG
  MOZ_CrashOOL(aFilename, aLine, sPrintfCrashReason);
#else
  MOZ_CrashOOL(nullptr, aLine, sPrintfCrashReason);
#endif
}

MOZ_END_EXTERN_C
