/**
 Simple logger that uses transaction observation
 */
module Set = Recoil_Observer.JsSet;

[@bs.val] [@bs.scope "console"] external group: string => unit = "group";
[@bs.val] [@bs.scope "console"] external groupCollapsed: string => unit = "groupCollapsed";
[@bs.val] [@bs.scope "console"] external groupEnd: unit => unit = "groupEnd";

let formatDate = date => {
  Js.Date.(
    Js.Float.toString(date->getHours)
    ++ ":"
    ++ Js.Float.toString(date->getMinutes)
    ++ ":"
    ++ Js.Float.toString(date->getSeconds)
  );
};

[@react.component]
let make = (~collapsed=true) => {
  Recoil_Observer.useTransactionObservation(({modifiedAtoms, atomValues, previousAtomValues, _} as e) =>
    modifiedAtoms
    ->Set.values
    ->Belt.Array.forEach(name => {
        //Js.log(e);
        let consoleGroup = collapsed ? groupCollapsed : group;
        consoleGroup(formatDate(Js.Date.make()) ++ " Atom name: " ++ name);
        Js.log2("new value", atomValues->Js.Dict.get(name));
        Js.log2("Previous value", previousAtomValues->Js.Dict.get(name));
        groupEnd();
      })
  );
  React.null;
};