let atom = {
  open Recoil.Atom
  value(~key="Persistence", ~default="", ~persistence="log", ())->make
}

module Item = {
  @react.component
  let make = (~item) => {
    let (selected, setSelected) = Recoil.useState(atom)

    <li
      onClick={_e => setSelected(._ => item)} className={selected == item ? "bg-red" : "bg-white"}>
      {item->React.string}
    </li>
  }
}

@react.component
let make = () =>
  <div>
    <ul>
      {["custom1", "custom2", "custom3", "custom4"]
      ->Belt.Array.map(item => <Item key=item item />)
      ->React.array}
    </ul>
  </div>