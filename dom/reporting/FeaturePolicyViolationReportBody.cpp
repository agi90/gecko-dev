/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/FeaturePolicyViolationReportBody.h"
#include "mozilla/dom/FeaturePolicyBinding.h"

namespace mozilla {
namespace dom {

FeaturePolicyViolationReportBody::FeaturePolicyViolationReportBody(nsPIDOMWindowInner* aWindow,
                                                                   const nsAString& aFeatureId,
                                                                   const nsAString& aSourceFile,
                                                                   const Nullable<int32_t>& aLineNumber,
                                                                   const Nullable<int32_t>& aColumnNumber,
                                                                   const nsAString& aDisposition)
  : ReportBody(aWindow)
  , mFeatureId(aFeatureId)
  , mSourceFile(aSourceFile)
  , mLineNumber(aLineNumber)
  , mColumnNumber(aColumnNumber)
  , mDisposition(aDisposition)
{}

FeaturePolicyViolationReportBody::~FeaturePolicyViolationReportBody() = default;

JSObject*
FeaturePolicyViolationReportBody::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
{
  return FeaturePolicyViolationReportBody_Binding::Wrap(aCx, this, aGivenProto);
}

void
FeaturePolicyViolationReportBody::GetFeatureId(nsAString& aFeatureId) const
{
  aFeatureId = mFeatureId;
}

void
FeaturePolicyViolationReportBody::GetSourceFile(nsAString& aSourceFile) const
{
  aSourceFile = mSourceFile;
}

Nullable<int32_t>
FeaturePolicyViolationReportBody::GetLineNumber() const
{
  return mLineNumber;
}

Nullable<int32_t>
FeaturePolicyViolationReportBody::GetColumnNumber() const
{
  return mColumnNumber;
}

void
FeaturePolicyViolationReportBody::GetDisposition(nsAString& aDisposition) const
{
  aDisposition = mDisposition;
}

} // dom namespace
} // mozilla namespace
