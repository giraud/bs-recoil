// Not defined by bucklescript
module JsSet = {
  type t<'a>
  @send external forEach: (t<'a>, 'a => unit) => unit = "forEach"
}

// Not defined by bucklescript
module JsMap = {
  type t<'a>
  @send external get: (t<'a>, string) => option<'a> = "get"
}

type mixed

type externallyVisibleAtomInfo

type callback = {
  atomValues: JsMap.t<mixed>,
  previousAtomValues: JsMap.t<mixed>,
  atomInfo: JsMap.t<externallyVisibleAtomInfo>,
  modifiedAtoms: JsSet.t<string>,
  transactionMetadata: Js.Dict.t<mixed>,
}

@module("recoil")
external useTransactionObservation: ((. callback) => unit) => unit =
  "useTransactionObservation_UNSTABLE"