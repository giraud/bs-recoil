// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "rescript/lib/es6/curry.js";
import * as React from "react";
import * as Recoil from "recoil";
import * as Belt_Array from "rescript/lib/es6/belt_Array.js";
import * as Caml_int32 from "rescript/lib/es6/caml_int32.js";
import * as Belt_Option from "rescript/lib/es6/belt_Option.js";

var id = {
  contents: 0
};

function getId(param) {
  id.contents = id.contents + 1 | 0;
  return id.contents;
}

var Todo = {};

function toString(x) {
  switch (x) {
    case /* ShowAll */0 :
        return "Show All";
    case /* ShowCompleted */1 :
        return "Show Completed";
    case /* ShowUncompleted */2 :
        return "Show Uncompleted";
    
  }
}

function fromString(x) {
  switch (x) {
    case "Show Completed" :
        return /* ShowCompleted */1;
    case "Show Uncompleted" :
        return /* ShowUncompleted */2;
    default:
      return /* ShowAll */0;
  }
}

var TodoStatus = {
  toString: toString,
  fromString: fromString
};

var todoListState = Recoil.atom({
      key: "todoListState",
      default: []
    });

var todoListFilterState = Recoil.atom({
      key: "todoListFilterState",
      default: /* ShowAll */0
    });

var filteredTodoListState = Recoil.selector({
      key: "filteredTodoListState",
      get: (function (param) {
          var get = param.get;
          var list = get(todoListState);
          var filter = get(todoListFilterState);
          switch (filter) {
            case /* ShowAll */0 :
                return list;
            case /* ShowCompleted */1 :
                return Belt_Array.keep(list, (function (item) {
                              return item.isComplete;
                            }));
            case /* ShowUncompleted */2 :
                return Belt_Array.keep(list, (function (item) {
                              return !item.isComplete;
                            }));
            
          }
        })
    });

var todoListStatsState = Recoil.selector({
      key: "todoListStatsState",
      get: (function (param) {
          var todoList = param.get(filteredTodoListState);
          var totalNum = todoList.length;
          var totalCompletedNum = Belt_Array.keep(todoList, (function (item) {
                  return item.isComplete;
                })).length;
          var totalUncompletedNum = totalNum - totalCompletedNum | 0;
          var percentCompleted = totalNum === 0 ? 0 : Caml_int32.div(totalCompletedNum, totalNum);
          return [
                  totalNum,
                  totalCompletedNum,
                  totalUncompletedNum,
                  percentCompleted
                ];
        })
    });

function AppTodoList$TodoItemCreator(Props) {
  var match = React.useState(function () {
        return "";
      });
  var setInputValue = match[1];
  var inputValue = match[0];
  var setTodoList = Recoil.useSetRecoilState(todoListState);
  var addItem = function (param) {
    return setTodoList(function (oldTodoList) {
                return Belt_Array.concat(oldTodoList, [{
                              id: getId(undefined),
                              text: inputValue,
                              isComplete: false
                            }]);
              });
  };
  var onChange = function (e) {
    var text = e.target.value;
    return Curry._1(setInputValue, (function (param) {
                  return text;
                }));
  };
  return React.createElement("div", undefined, React.createElement("input", {
                  type: "text",
                  value: inputValue,
                  onChange: onChange
                }), React.createElement("button", {
                  onClick: addItem
                }, "Add"));
}

var TodoItemCreator = {
  make: AppTodoList$TodoItemCreator
};

function AppTodoList$TodoItem(Props) {
  var item = Props.item;
  var match = Recoil.useRecoilState(todoListState);
  var setTodoList = match[1];
  var todoList = match[0];
  var index = Belt_Option.getWithDefault(Belt_Array.getIndexBy(todoList, (function (listItem) {
              return listItem === item;
            })), -1);
  var editItemText = function ($$event) {
    var text = $$event.target.value;
    return setTodoList(function (param) {
                return Belt_Array.mapWithIndex(todoList, (function (i, item) {
                              if (i === index) {
                                return {
                                        id: item.id,
                                        text: text,
                                        isComplete: item.isComplete
                                      };
                              } else {
                                return item;
                              }
                            }));
              });
  };
  var toggleItemCompletion = function (param) {
    return setTodoList(function (param) {
                return Belt_Array.mapWithIndex(todoList, (function (i, item) {
                              if (i === index) {
                                return {
                                        id: item.id,
                                        text: item.text,
                                        isComplete: !item.isComplete
                                      };
                              } else {
                                return item;
                              }
                            }));
              });
  };
  var deleteItem = function (param) {
    return setTodoList(function (param) {
                return Belt_Array.keepWithIndex(todoList, (function (param, i) {
                              return i !== index;
                            }));
              });
  };
  return React.createElement("div", undefined, React.createElement("input", {
                  type: "text",
                  value: item.text,
                  onChange: editItemText
                }), React.createElement("input", {
                  checked: item.isComplete,
                  type: "checkbox",
                  onChange: toggleItemCompletion
                }), React.createElement("button", {
                  onClick: deleteItem
                }, "X"));
}

var TodoItem = {
  make: AppTodoList$TodoItem
};

function AppTodoList$TodoListFilters(Props) {
  var match = Recoil.useRecoilState(todoListFilterState);
  var setFilter = match[1];
  var updateFilter = function ($$event) {
    var value = $$event.target.value;
    return setFilter(function (param) {
                return fromString(value);
              });
  };
  return React.createElement(React.Fragment, undefined, "Filter:", React.createElement("select", {
                  value: toString(match[0]),
                  onChange: updateFilter
                }, Belt_Array.map([
                      /* ShowAll */0,
                      /* ShowCompleted */1,
                      /* ShowUncompleted */2
                    ], (function (opt) {
                        return React.createElement("option", {
                                    key: toString(opt),
                                    value: toString(opt)
                                  }, toString(opt));
                      }))));
}

var TodoListFilters = {
  make: AppTodoList$TodoListFilters
};

function AppTodoList$TodoStats(Props) {
  var match = Recoil.useRecoilValue(todoListStatsState);
  var formattedPercentCompleted = Math.round(Math.imul(match[3], 100));
  return React.createElement("ul", undefined, React.createElement("li", undefined, "Total items: " + match[0].toString()), React.createElement("li", undefined, "Items completed: " + match[1].toString()), React.createElement("li", undefined, "Items not completed: " + match[2].toString()), React.createElement("li", undefined, "Percent completed: " + formattedPercentCompleted.toString()));
}

var TodoStats = {
  make: AppTodoList$TodoStats
};

function AppTodoList(Props) {
  var todoList = Recoil.useRecoilValue(filteredTodoListState);
  return React.createElement(React.Fragment, undefined, React.createElement(AppTodoList$TodoItemCreator, {}), React.createElement(AppTodoList$TodoListFilters, {}), React.createElement(AppTodoList$TodoStats, {}), Belt_Array.map(todoList, (function (item) {
                    return React.createElement(AppTodoList$TodoItem, {
                                item: item,
                                key: String(item.id)
                              });
                  })));
}

var make = AppTodoList;

export {
  id ,
  getId ,
  Todo ,
  TodoStatus ,
  todoListState ,
  todoListFilterState ,
  filteredTodoListState ,
  todoListStatsState ,
  TodoItemCreator ,
  TodoItem ,
  TodoListFilters ,
  TodoStats ,
  make ,
  
}
/* todoListState Not a pure module */