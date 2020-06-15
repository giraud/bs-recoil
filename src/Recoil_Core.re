module Atom = {
  type t('a);

  type init('a) = {
    key: string,
    default: 'a,
    //    persistence_UNSTABLE: persistence
  };

  [@bs.module "recoil"] external make: init('a) => t('a) = "atom";

  module Family = {
    [@bs.module "recoil"] external make: init('a) => (. 'familyId) => t('a) = "atomFamily";
  };
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

  module Family = {
    type initGet('param, 'a) = {
      key: string,
      get: (. 'param) => (. Props.get) => 'a,
    };

    type initGetSet('param, 'a) = {
      key: string,
      get: (. 'param) => (. Props.get) => 'a,
      set: (. 'param) => (. Props.set, 'a) => unit,
    };

    [@bs.module "recoil"] external makeGet: initGet('param, 'a) => (. 'param) => Atom.t('a) = "selectorFamily";

    [@bs.module "recoil"] external makeGetSet: initGetSet('param, 'a) => (. 'param) => Atom.t('a) = "selectorFamily";
  };

  [@bs.module "recoil"] external makeGet: initGet('a) => Atom.t('a) = "selector";

  [@bs.module "recoil"] external makeGetSet: initGetSet('a) => Atom.t('a) = "selector";
};