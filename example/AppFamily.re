let listFamily = Recoil.Atom.Family.make({key: "List", default: ([||]: array(string))});

// Helpers
let isSelected = (items, item) => items->Belt.Array.getBy(i => i == item)->Belt.Option.isSome;
let add = (items, item) => items->Belt.Array.concat([|item|]);
let remove = (items, item) => items->Belt.Array.keep(i => i != item);

module Item = {
  [@react.component]
  let make = (~item, ~atomFamily) => {
    let (selected, setSelected) = Recoil.useState(atomFamily(. item));

    <li onClick={_e => setSelected(. !selected)} className={selected ? "bg-red" : "bg-white"}>
      item->React.string
    </li>;
  };
};

module List = {
  [@react.component]
  let make = (~id, ~items) => {
    let listAtom = listFamily(. id);
    let rwItemSelector =
      Recoil.Selector.Family.makeGetSet({
        key: "ListSelection",
        get: (. item) => (. {get}) => get(. listAtom)->isSelected(item),
        set:
          (. item) =>
            (. {get, set}, selected) => {
              let x = get(. listAtom);
              set(. listAtom, selected ? x->add(item) : x->remove(item));
            },
      });

    <div>
      {("List " ++ id)->React.string}
      <ul> {items->Belt.Array.map(item => <Item key=item item atomFamily=rwItemSelector />)->React.array} </ul>
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