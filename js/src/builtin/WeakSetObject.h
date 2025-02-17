/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef builtin_WeakSetObject_h
#define builtin_WeakSetObject_h

#include "builtin/WeakMapObject.h"

namespace js {

class GlobalObject;

class WeakSetObject : public WeakCollectionObject
{
  public:
    static const Class class_;
    static const Class protoClass_;

  private:
    static const ClassSpec classSpec_;

    static const JSPropertySpec properties[];
    static const JSFunctionSpec methods[];

    static WeakSetObject* create(JSContext* cx, HandleObject proto = nullptr);
    static MOZ_MUST_USE bool construct(JSContext* cx, unsigned argc, Value* vp);

    static MOZ_MUST_USE MOZ_ALWAYS_INLINE bool is(HandleValue v);

    static MOZ_MUST_USE MOZ_ALWAYS_INLINE bool add_impl(JSContext* cx, const CallArgs& args);
    static MOZ_MUST_USE bool add(JSContext* cx, unsigned argc, Value* vp);
    static MOZ_MUST_USE MOZ_ALWAYS_INLINE bool delete_impl(JSContext* cx, const CallArgs& args);
    static MOZ_MUST_USE bool delete_(JSContext* cx, unsigned argc, Value* vp);
    static MOZ_MUST_USE MOZ_ALWAYS_INLINE bool has_impl(JSContext* cx, const CallArgs& args);
    static MOZ_MUST_USE bool has(JSContext* cx, unsigned argc, Value* vp);

    static bool isBuiltinAdd(HandleValue add);
};

} // namespace js

template<>
inline bool
JSObject::is<js::WeakCollectionObject>() const
{
    return is<js::WeakMapObject>() || is<js::WeakSetObject>();
}

#endif /* builtin_WeakSetObject_h */
