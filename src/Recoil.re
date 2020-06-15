include Recoil_Core;

[@bs.module "recoil"]
external useState: Atom.t('a) => ('a, (. 'a) => unit) = "useRecoilState";

[@bs.module "recoil"] external useValue: Atom.t('a) => 'a = "useRecoilValue";

type setter('a) = (. ('a => 'a)) => unit;

[@bs.module "recoil"]
external useSetState: Atom.t('a) => setter('a) = "useSetRecoilState";

module Root = Recoil_Root;
module Logger = Recoil_Logger;