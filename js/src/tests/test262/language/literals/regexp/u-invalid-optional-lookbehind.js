// |reftest| error:SyntaxError
// Copyright (C) 2018 Igalia S. L. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.
/*---
esid: sec-regular-expressions-patterns
description: Lookbehinds are not treated as a QuantifiableAssertion
info: |
    Term[U] ::
         [~U] QuantifiableAssertion Quantifier

    QuantifiableAssertion[N]::
        ( ? = Disjunction[~U, ?N] )
        ( ? ! Disjunction[~U, ?N] )
negative:
  phase: parse
  type: SyntaxError
---*/

throw "Test262: This statement should not be evaluated.";

/.(?<=.)?/u;
