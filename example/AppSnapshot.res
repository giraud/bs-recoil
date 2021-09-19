let itemsInCart = {
  open Recoil.Atom
  value(~key="ItemsInCart", ~default=([]: array<string>), ~persistence="log", ())->make
}

module TimeTravelObserver = {
  @react.component
  let make = () => {
    let (snapshots, setSnapshots) = React.useState(() => [])
    let gotoSnapshot = Recoil.useGotoSnapshot()
    Recoil.useTransactionObserver((. {snapshot, _}) =>
      setSnapshots(prev => prev->Belt.Array.concat([snapshot]))
    )

    <ol>
      {snapshots
      ->Belt.Array.mapWithIndexU((. i, snapshot) =>
        <li key={string_of_int(i)}>
          {("Snapshot " ++ string_of_int(i))->React.string}
          <button onClick={_ => gotoSnapshot(. snapshot)}> {"Restore"->React.string} </button>
        </li>
      )
      ->React.array}
    </ol>
  }
}

@react.component
let make = () => {
  let (items, setItems) = Recoil.useState(itemsInCart)
  let addItem = _e => setItems(.prev => prev->Belt.Array.concat(["item"]))

  let snapshot = Recoil.useSnapshot()
  Js.log2("Snapshot", snapshot)

  let logCartItems = Recoil.useCallback0(({snapshot, _}, . _) =>
    switch snapshot->Recoil.Snapshot.get(itemsInCart) {
    | HasValue(x) => Js.log2("items", x->Belt.Array.length)
    | Loading => Js.log("loading...")
    | Error(x) => Js.log2("error", x)
    }
  )
  <div>
    <ol>
      {items
      ->Belt.Array.mapWithIndexU((. i, item) =>
        <li key={string_of_int(i)}> {item->React.string} </li>
      )
      ->React.array}
    </ol>
    <button onClick={e => logCartItems(. e)}> {"Log cart items"->React.string} </button>
    <button onClick=addItem> {"Add item"->React.string} </button>
    <TimeTravelObserver />
  </div>
}

