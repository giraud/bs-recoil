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
      set: 'a. (. Atom.t('a), 'a => 'a) => unit,
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

[@bs.module "recoil"] external useState: Atom.t('a) => ('a, (. 'a) => unit) = "useRecoilState";

[@bs.module "recoil"] external useValue: Atom.t('a) => 'a = "useRecoilValue";

type setter('a) = (. ('a => 'a)) => unit;

[@bs.module "recoil"] external useSetState: Atom.t('a) => setter('a) = "useSetRecoilState";

module Root = Recoil_Root;
module Logger = Recoil_Logger;