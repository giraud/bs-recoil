let itemsInCart =
  Recoil.Atom.(value(~key="ItemsInCart", ~default=[||]: array(string), ~persistence="log", ())->make);

[@react.component]
let make = () => {
  let setItems = Recoil.useSetState(itemsInCart);
  let addItem = _e => setItems(. prev => prev->Belt.Array.concat([|"item"|]));

  let logCartItems =
    Recoil.useCallback0(({snapshot, _}) =>
      (. _) => {
        switch (snapshot->Recoil.Snapshot.get(itemsInCart)) {
        | HasValue(x) => Js.log2("items", x->Belt.Array.length)
        | Loading => Js.log("loading...")
        | Error(x) => Js.log2("error", x)
        };
      }
    );
  <div>
    <button onClick={e => logCartItems(. e)}> "Log cart items"->React.string </button>
    <button onClick=addItem> "Add item"->React.string </button>
  </div>;
};