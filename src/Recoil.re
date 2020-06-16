type stateSetter('a) = 'a => 'a;

module Atom = {
  type t('a);

  [@bs.deriving abstract]
  type value('a) = {
    key: string,
    default: 'a,
    [@bs.as "persistence_UNSTABLE"] [@bs.optional]
    persistence: string,
  };

  [@bs.module "recoil"] external make: value('a) => t('a) = "atom";

  module Family = {
    [@bs.module "recoil"] external make: value('a) => (. 'familyId) => t('a) = "atomFamily";
  };
};

module Selector = {
  type t('a);

  module Props = {
    type get = {get: 'a. (. Atom.t('a)) => 'a};

    type set = {
      get: 'a. (. Atom.t('a)) => 'a,
      set: 'a. (. Atom.t('a), stateSetter('a)) => unit,
    };
  };

  [@bs.deriving abstract]
  type value('a) = {
    key: string,
    get: Props.get => 'a,
    [@bs.optional]
    set: (Props.set, 'a) => unit,
  };

  module Family = {
    [@bs.deriving abstract]
    type value('param, 'a) = {
      key: string,
      get: (. 'param) => (. Props.get) => 'a,
      [@bs.optional]
      set: (. 'param) => (. Props.set, 'a) => unit,
    };

    [@bs.module "recoil"] external make: value('param, 'a) => (. 'param) => Atom.t('a) = "selectorFamily";
  };

  [@bs.module "recoil"] external make: value('a) => Atom.t('a) = "selector";
};

/**
 Returns a tuple where the first element is the value of state and
 the second element is a setter function that will update the value of the given state when called.

 This API is similar to the React useState() hook except
 it takes a Recoil state instead of a default value as an argument.
 */
[@bs.module "recoil"]
external useState: Atom.t('a) => ('a, (. stateSetter('a)) => unit) = "useRecoilState";

/**
 Returns the value of the given Recoil state.

 This is the recommended hook to use when a component intends to read state without writing to it,
 as this hook works with both read-only state and writeable state.
 */
[@bs.module "recoil"]
external useValue: Atom.t('a) => 'a = "useRecoilValue";

[@bs.module "recoil"] external useSetState: Atom.t('a) => (. stateSetter('a)) => unit = "useSetRecoilState";

module Root = Recoil_Root;
module Logger = Recoil_Logger;