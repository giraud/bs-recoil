/**
 Simple logger that uses transaction observation
 */
 
module Set = Recoil_Observer.JsSet
module Map = Recoil_Observer.JsMap

@bs.val @bs.scope("console") external group: (. string) => unit = "group"
@bs.val @bs.scope("console") external groupCollapsed: (. string) => unit = "groupCollapsed"
@bs.val @bs.scope("console") external groupEnd: unit => unit = "groupEnd"

let formatDate = date => {
  open Js.Date
  Js.Float.toString(date->getHours)
    ++ ":"
    ++ Js.Float.toString(date->getMinutes)
    ++ ":"
    ++ Js.Float.toString(date->getSeconds)
}

@react.component
let make = (~collapsed=true) => {
  Recoil_Observer.useTransactionObservation((. {modifiedAtoms, atomValues, previousAtomValues, _} /*as e*/) =>
    modifiedAtoms->Set.forEach(name => {
      //Js.log(e)
      let consoleGroup = collapsed ? groupCollapsed : group
      consoleGroup(. formatDate(Js.Date.make()) ++ " " ++ name)
      Js.log2("Previous:", previousAtomValues->Map.get(name))
      Js.log2("New:", atomValues->Map.get(name))
      groupEnd()
    })
  )
  React.null
}