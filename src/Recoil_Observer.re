// Not defined by bucklescript !
module JsSet = {
  type t('a);
  type iterable('a);

  [@bs.new] external make: array('a) => t('a) = "Set";
  [@bs.get] external size: t('a) => float = "size";
  [@bs.send] external add: (t('a), 'a) => unit = "add";
  [@bs.send] external delete: (t('a), 'a) => unit = "delete";
  [@bs.send] external has: (t('a), 'a) => bool = "has";
  [@bs.send] external rawValues: t('a) => iterable('a) = "values";
  [@bs.val] [@bs.scope "Array"] external arrayFromIterable: iterable('a) => array('a) = "from";

  let values = (set: t('a)): array('a) => arrayFromIterable(rawValues(set));
};

type mixed;

type externallyVisibleAtomInfo;

type callback = {
  atomValues: Js.Dict.t(mixed),
  previousAtomValues: Js.Dict.t(mixed),
  atomInfo: Js.Dict.t(externallyVisibleAtomInfo),
  modifiedAtoms: JsSet.t(string),
  transactionMetadata: Js.Dict.t(mixed),
};

[@bs.module "recoil"]
external useTransactionObservation: ([@bs.uncurry] (callback => unit)) => unit = "useTransactionObservation_UNSTABLE";