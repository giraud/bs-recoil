module Root = {
  [@bs.module "recoil"] [@react.component]
  external make: (~children: React.element) => React.element = "RecoilRoot";
};

module Atom = {
  type t('a);

  type init('a) = {
    key: string,
    default: 'a,
  };

  [@bs.module "recoil"] external make: init('a) => t('a) = "atom";
};

module Selector = {
  type t('a);

  module Props = {
    type get = {get: 'a. (. Atom.t('a)) => 'a};

    type set = {
      get: 'a. (. Atom.t('a)) => 'a,
      set: 'a. (. Atom.t('a), 'a) => unit,
    };
  };

  type initGet('a) = {
    key: string,
    get: Props.get => 'a,
  };

  type initGetSet('a) = {
    key: string,
    get: Props.get => 'a,
    set: (Props.set, 'a) => unit,
  };

  [@bs.module "recoil"]
  external makeGet: initGet('a) => Atom.t('a) = "selector";

  [@bs.module "recoil"]
  external makeGetSet: initGetSet('a) => Atom.t('a) = "selector";
};

[@bs.module "recoil"]
external useState: Atom.t('a) => ('a, (. 'a) => unit) = "useRecoilState";

[@bs.module "recoil"] external useValue: Atom.t('a) => 'a = "useRecoilValue";

type setter('a) = (. ('a => 'a)) => unit;
[@bs.module "recoil"]
external useSetState: Atom.t('a) => setter('a) = "useSetRecoilState";