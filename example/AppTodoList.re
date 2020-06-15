// utility for creating unique Id
let id = ref(0);
let getId = () => {
  id := id^ + 1;
  id^;
};

module Todo = {
  type t = {
    id: int,
    text: string,
    isComplete: bool,
  };
};

module TodoStatus = {
  type t =
    | ShowAll
    | ShowCompleted
    | ShowUncompleted;

  let toString =
    fun
    | ShowAll => "Show All"
    | ShowCompleted => "Show Completed"
    | ShowUncompleted => "Show Uncompleted";

  let fromString =
    fun
    | "Show Completed" => ShowCompleted
    | "Show Uncompleted" => ShowUncompleted
    | _ => ShowAll;
};

// Atoms

let todoListState: Recoil.Atom.t(array(Todo.t)) =
  Recoil.Atom.value(~key="todoListState", ~default=[||], ())->Recoil.Atom.make;
let todoListFilterState =
  Recoil.Atom.value(~key="todoListFilterState", ~default=TodoStatus.ShowAll, ())->Recoil.Atom.make;

// Selectors

let filteredTodoListState =
  Recoil.Selector.value(
    ~key="filteredTodoListState",
    ~get=
      ({get}) => {
        let list = get(. todoListState);
        let filter = get(. todoListFilterState);
        switch (filter) {
        | ShowCompleted => list->Belt.Array.keep(item => item.isComplete)
        | ShowUncompleted => list->Belt.Array.keep(item => !item.isComplete)
        | ShowAll => list
        };
      },
    (),
  )
  ->Recoil.Selector.make;

let todoListStatsState =
  Recoil.Selector.value(
    ~key="todoListStatsState",
    ~get=
      ({get}) => {
        let todoList = get(. filteredTodoListState);
        open Belt.Array;

        let totalNum = todoList->length;
        let totalCompletedNum = todoList->keep(item => item.isComplete)->length;
        let totalUncompletedNum = totalNum - totalCompletedNum;
        let percentCompleted = totalNum === 0 ? 0 : totalCompletedNum / totalNum;

        (totalNum, totalCompletedNum, totalUncompletedNum, percentCompleted);
      },
    (),
  )
  ->Recoil.Selector.make;

// Components

module TodoItemCreator = {
  [@react.component]
  let make = () => {
    let (inputValue, setInputValue) = React.useState(() => "");
    let setTodoList = Recoil.useSetState(todoListState);

    let addItem = _ =>
      setTodoList(. oldTodoList =>
        oldTodoList->Belt.Array.concat([|{id: getId(), text: inputValue, isComplete: false}|])
      );

    let onChange = e => {
      let text: string = e->ReactEvent.Form.target##value;
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
      let text: string = event->ReactEvent.Form.target##value;
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
      let value: string = event->ReactEvent.Form.target##value;
      setFilter(. TodoStatus.fromString(value));
    };

    TodoStatus.(
      <>
        "Filter:"->React.string
        <select value={filter->toString} onChange=updateFilter>
          {[|ShowAll, ShowCompleted, ShowUncompleted|]
           ->Belt.Array.map(opt =>
               <option key={opt->toString} value={opt->toString}> {opt->toString->React.string} </option>
             )
           ->React.array}
        </select>
      </>
    );
  };
};

module TodoStats = {
  [@react.component]
  let make = () => {
    let (totalNum, totalCompletedNum, totalUncompletedNum, percentCompleted) = Recoil.useValue(todoListStatsState);
    let formattedPercentCompleted = Js.Math.round(Js.Int.toFloat(percentCompleted * 100));

    <ul>
      <li> {("Total items: " ++ Js.Int.toString(totalNum))->React.string} </li>
      <li> {("Items completed: " ++ Js.Int.toString(totalCompletedNum))->React.string} </li>
      <li> {("Items not completed: " ++ Js.Int.toString(totalUncompletedNum))->React.string} </li>
      <li> {("Percent completed: " ++ Js.Float.toString(formattedPercentCompleted))->React.string} </li>
    </ul>;
  };
};

[@react.component]
let make = () => {
  let todoList = Recoil.useValue(filteredTodoListState);

  <>
    <TodoItemCreator />
    <TodoListFilters />
    <TodoStats />
    {todoList->Belt.Array.map(item => <TodoItem key={string_of_int(item.id)} item />)->React.array}
  </>;
};