// Helpers
let isSelected = (items, item) =>
  items->Belt.Array.getBy(i => i == item)->Belt.Option.isSome;
let add = (items, item) => items->Belt.Array.concat([|item|]);
let remove = (items, item) => items->Belt.Array.keep(i => i != item);

// An atom associated to a particular instance of a list
let listFamily =
  Recoil.Atom.value(~key="List", ~default=[||]: array(string), ())
  ->Recoil.Atom.Family.make;

module Item = {
  [@react.component]
  let make = (~item, ~atom) => {
    let (selected, setSelected) = Recoil.useState(atom(. item));

    <li
      onClick={_e => setSelected(. _ => !selected)}
      className={selected ? "bg-red" : "bg-white"}>
      item->React.string
    </li>;
  };
};

module List = {
  [@react.component]
  let make = (~id, ~items) => {
    let listAtom = listFamily(. id);
    let rwItemSelector =
      Recoil.Selector.Family.value(
        ~key="ListSelection",
        ~get=(. item) => (. {get}) => get(. listAtom)->isSelected(item),
        ~set=
          (. item) =>
            (. {set, _}, selected) => {
              set(. listAtom, previous =>
                selected ? previous->add(item) : previous->remove(item)
              );
            },
        (),
      )
      ->Recoil.Selector.Family.make;

    <div>
      {("List " ++ id)->React.string}
      <ul>
        {items
         ->Belt.Array.map(item => <Item key=item item atom=rwItemSelector />)
         ->React.array}
      </ul>
    </div>;
  };
};

[@react.component]
let make = () => {
  <div>
    <List id="A" items=[|"item1", "item2", "item3", "item4"|] />
    <List id="B" items=[|"item1", "item2", "item3", "item4"|] />
  </div>;
};