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

module Selector = {
  type t;

  // how to zero cost and type safe ?
  let apply = [%raw {| function(g,a) { return g(a); } |}];
  external unsafeAtom: 'a => Atom.t('b) = "%identity";

  type init('a, 'b) = {
    key: string,
    get: 'b => 'a,
  };

  [@bs.module "recoil"] external makeGetter: init('a, 'b) => t = "selector";
};

type stateSetter('a) = (. 'a) => unit;
[@bs.module "recoil"] external useState: Atom.t('a) => ('a, stateSetter('a)) = "useRecoilState";

[@bs.module "recoil"] external useValue: Atom.t('a) => 'a = "useRecoilValue";

type setter('a) = ((. 'a) => 'a) => unit;
[@bs.module "recoil"] external useSetState: Atom.t('a) => setter('a) = "useSetRecoilState";