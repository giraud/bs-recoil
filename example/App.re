// utility for creating unique Id
let id = ref(0);
let getId = () => {
  id := id^ + 1;
  id^;
};

type todo = {
  id: int,
  text: string,
  isComplete: bool,
};

let todoListState: Recoil.Atom.t(array(todo)) = Recoil.Atom.make({key: "todoListState", default: [||]});

module TodoItemCreator = {
  [@react.component]
  let make = () => {
    let (inputValue, setInputValue) = React.useState(() => "");
    let setTodoList = Recoil.useSetState(todoListState);

    let addItem = _ =>
      setTodoList((. oldTodoList) =>
        oldTodoList->Belt.Array.concat([|{id: getId(), text: inputValue, isComplete: false}|])
      );

    let onChange = e => {
      let text = e->ReactEvent.Form.target##value;
      setInputValue(_ => text);
    };

    <div>
      <input type_="text" value=inputValue onChange />
      <button onClick=addItem> "Add"->React.string </button>
    </div>;
  };
};

module TodoItem = {
  [@react.component]
  let make = (~item) => {
    let (todoList, setTodoList) = Recoil.useState(todoListState);
    let index = todoList->Belt.Array.getIndexBy(listItem => listItem === item)->Belt.Option.getWithDefault(-1);

    let editItemText = event => {
      let text = event->ReactEvent.Form.target##value;
      setTodoList(. todoList->Belt.Array.mapWithIndex((i, item) => i == index ? {...item, text} : item));
    };

    let toggleItemCompletion = _ =>
      setTodoList(.
        todoList->Belt.Array.mapWithIndex((i, item) => i == index ? {...item, isComplete: !item.isComplete} : item),
      );

    let deleteItem = _ => setTodoList(. todoList->Belt.Array.keepWithIndex((_, i) => i != index));

    <div>
      <input type_="text" value={item.text} onChange=editItemText />
      <input type_="checkbox" checked={item.isComplete} onChange=toggleItemCompletion />
      <button onClick=deleteItem> "X"->React.string </button>
    </div>;
  };
};

module TodoList = {
  [@react.component]
  let make = () => {
    let todoList = Recoil.useValue(todoListState);

    <>
      <TodoItemCreator />
      {todoList->Belt.Array.map(item => <TodoItem key={string_of_int(item.id)} item />)->React.array}
    </>;
  };
};

ReactDOMRe.renderToElementWithId(<Recoil.Root> <TodoList /> </Recoil.Root>, "app");