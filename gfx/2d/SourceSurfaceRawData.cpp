/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SourceSurfaceRawData.h"

#include "DataSurfaceHelpers.h"
#include "Logging.h"
#include "mozilla/Types.h" // for decltype

namespace mozilla {
namespace gfx {

void
SourceSurfaceRawData::InitWrappingData(uint8_t *aData,
                                       const IntSize &aSize,
                                       int32_t aStride,
                                       SurfaceFormat aFormat,
                                       Factory::SourceSurfaceDeallocator aDeallocator,
                                       void* aClosure)
{
  mRawData = aData;
  mSize = aSize;
  mStride = aStride;
  mFormat = aFormat;

  if (aDeallocator) {
    mOwnData = true;
  }
  mDeallocator = aDeallocator;
  mClosure = aClosure;
}

void
SourceSurfaceRawData::GuaranteePersistance()
{
  if (mOwnData) {
    return;
  }

  MOZ_ASSERT(!mDeallocator);
  uint8_t* oldData = mRawData;
  mRawData = new uint8_t[mStride * mSize.height];

  memcpy(mRawData, oldData, mStride * mSize.height);
  mOwnData = true;
}

bool
SourceSurfaceAlignedRawData::Init(const IntSize &aSize,
                                  SurfaceFormat aFormat,
                                  bool aClearMem,
                                  uint8_t aClearValue,
                                  int32_t aStride)
{
  mFormat = aFormat;
  mStride = aStride ? aStride : GetAlignedStride<16>(aSize.width, BytesPerPixel(aFormat));

  size_t bufLen = BufferSizeFromStrideAndHeight(mStride, aSize.height);
  if (bufLen > 0) {
    bool zeroMem = aClearMem && !aClearValue;
    static_assert(sizeof(decltype(mArray[0])) == 1,
                  "mArray.Realloc() takes an object count, so its objects must be 1-byte sized if we use bufLen");

    // AlignedArray uses cmalloc to zero mem for a fast path.
    mArray.Realloc(/* actually an object count */ bufLen, zeroMem);
    mSize = aSize;

    if (mArray && aClearMem && aClearValue) {
      memset(mArray, aClearValue, mStride * aSize.height);
    }
  } else {
    mArray.Dealloc();
    mSize.SizeTo(0, 0);
  }

  return mArray != nullptr;
}

void
SourceSurfaceAlignedRawData::AddSizeOfExcludingThis(MallocSizeOf aMallocSizeOf,
                                                    size_t& aHeapSizeOut,
                                                    size_t& aNonHeapSizeOut,
                                                    size_t& aExtHandlesOut,
                                                    uint64_t& aExtIdOut) const
{
  aHeapSizeOut += mArray.HeapSizeOfExcludingThis(aMallocSizeOf);
}

} // namespace gfx
} // namespace mozilla
