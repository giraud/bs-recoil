# bs-recoil

Experimental ReasonML bindings to [recoil](https://github.com/facebookexperimental/recoil).

## Usage 

Create an atom:

```reason
let todoListState = Recoil.Atom.value(~key="todoListState", ~default=[||]: array(Todo.t), ())->Recoil.Atom.make;
```

Create a getter:
```reason

let filteredTodoListState =
  Recoil.Selector.value(
    ~key="filteredTodoListState",
    ~get=({get}) => { ... },
    (),
  )
  ->Recoil.Selector.make;
```

Create a setter:
```reason

let filteredTodoListState =
  Recoil.Selector.value(
    ~key="filteredTodoListState",
    ~get=({get}) => { ... },
    ~set=({get, set}) => { ... },
    (),
  )
  ->Recoil.Selector.make;
```

## Dev

- clone this directory
- yarn install
- yarn dev
- go to http://localhost:1234

example directory contains the todo app from the recoil doc
