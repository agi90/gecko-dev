/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_dom_SharedWorkerParent_h
#define mozilla_dom_dom_SharedWorkerParent_h

#include "mozilla/dom/PSharedWorkerParent.h"
#include "mozilla/ipc/BackgroundUtils.h"
#include "nsISupportsImpl.h"

namespace mozilla {
namespace dom {

class MessagePortIdentifier;
class RemoteWorkerData;
class SharedWorkerManager;
class SharedWorkerService;

class SharedWorkerParent final : public mozilla::dom::PSharedWorkerParent
{
public:
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(SharedWorkerParent)

  SharedWorkerParent();

  void
  Initialize(const RemoteWorkerData& aData,
             uint64_t aWindowID,
             const MessagePortIdentifier& aPortIdentifier);

  void
  ManagerCreated(SharedWorkerManager* aWorkerManager);

  void
  ErrorPropagation(nsresult aError);

  mozilla::ipc::IPCResult
  RecvClose() override;

  mozilla::ipc::IPCResult
  RecvSuspend() override;

  mozilla::ipc::IPCResult
  RecvResume() override;

  mozilla::ipc::IPCResult
  RecvFreeze() override;

  mozilla::ipc::IPCResult
  RecvThaw() override;

  bool
  IsSuspended() const
  {
    return mSuspended;
  }

  bool
  IsFrozen() const
  {
    return mFrozen;
  }

  uint64_t
  WindowID() const
  {
    return mWindowID;
  }

private:
  ~SharedWorkerParent();

  void
  ActorDestroy(IProtocol::ActorDestroyReason aReason) override;

  nsCOMPtr<nsIEventTarget> mBackgroundEventTarget;
  RefPtr<SharedWorkerManager> mWorkerManager;
  RefPtr<SharedWorkerService> mService;

  enum {
    eInit,
    ePending,
    eActive,
    eClosed,
  } mStatus;

  uint64_t mWindowID;

  bool mSuspended;
  bool mFrozen;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_dom_SharedWorkerParent_h
