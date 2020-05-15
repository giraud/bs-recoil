module Root = {
  [@bs.module "recoil"] [@react.component] external make: (~children: React.element) => React.element = "RecoilRoot";
};

module Atom = {
  type t('a);

  type init('a) = {
    key: string,
    default: 'a,
  };

  [@bs.module "recoil"] external make: init('a) => t('a) = "atom";
};

type stateSetter('a) = (. 'a) => unit;
[@bs.module "recoil"] external useState: Atom.t('a) => ('a, stateSetter('a)) = "useRecoilState";

[@bs.module "recoil"] external useValue: Atom.t('a) => 'a = "useRecoilValue";

type setter('a) = ((. 'a) => 'a) => unit;
[@bs.module "recoil"] external useSetState: Atom.t('a) => setter('a) = "useSetRecoilState";