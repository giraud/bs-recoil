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

type todoStatus =
  | ShowAll
  | ShowCompleted
  | ShowUncompleted;

let statusToString =
  fun
  | ShowAll => "Show all"
  | ShowCompleted => "Show completed"
  | ShowUncompleted => "Show uncompleted";

// Atoms

let todoListState: Recoil.Atom.t(array(todo)) = Recoil.Atom.make({key: "todoListState", default: [||]});
let todoListFilterState = Recoil.Atom.make({key: "todoListFilterState", default: ShowAll});

// Selectors

let filteredTodoListState =
  Recoil.Selector.makeGetter({
    key: "filteredTodoListState",
    get: (Recoil.Selector.{get}) => {
      let list = get->Recoil.Selector.apply(todoListState);
      let filter = get->Recoil.Selector.apply(todoListFilterState);

      switch (filter) {
      | ShowCompleted => list->Belt.Array.keep(item => item.isComplete)
      | ShowUncompleted => list->Belt.Array.keep(item => !item.isComplete)
      | ShowAll => list
      };
    },
  });

// Components

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

module TodoListFilters = {
  [@react.component]
  let make = () => {
    let (filter, setFilter) = Recoil.useState(todoListFilterState);

    let updateFilter = event => {
      let value = event->ReactEvent.Form.target##value;
      setFilter(. value);
    };

    <>
      "Filter:"->React.string
      <select value={filter->statusToString} onChange=updateFilter>
        <option value="Show All"> "All"->React.string </option>
        <option value="Show Completed"> "Completed"->React.string </option>
        <option value="Show Uncompleted"> "Uncompleted"->React.string </option>
      </select>
    </>;
  };
};

module TodoList = {
  [@react.component]
  let make = () => {
    let todoList = Recoil.useValue(filteredTodoListState->Recoil.Selector.unsafeAtom);

    <>
      <TodoItemCreator />
      <TodoListFilters />
      {todoList->Belt.Array.map(item => <TodoItem key={string_of_int(item.id)} item />)->React.array}
    </>;
  };
};

// Root

ReactDOMRe.renderToElementWithId(<Recoil.Root> <TodoList /> </Recoil.Root>, "app");