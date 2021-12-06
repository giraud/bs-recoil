// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "rescript/lib/es6/curry.js";
import * as React from "react";
import * as Recoil from "../src/Recoil.js";
import * as Recoil$1 from "recoil";
import * as Belt_Array from "rescript/lib/es6/belt_Array.js";

var itemsInCart = Recoil$1.atom({
      key: "ItemsInCart",
      default: [],
      persistence_UNSTABLE: "log"
    });

function AppSnapshot$TimeTravelObserver(Props) {
  var match = React.useState(function () {
        return [];
      });
  var setSnapshots = match[1];
  var gotoSnapshot = Recoil$1.useGotoRecoilSnapshot();
  Recoil$1.useRecoilTransactionObserver_UNSTABLE(function (param) {
        var snapshot = param.snapshot;
        return Curry._1(setSnapshots, (function (prev) {
                      return Belt_Array.concat(prev, [snapshot]);
                    }));
      });
  return React.createElement("ol", undefined, Belt_Array.mapWithIndexU(match[0], (function (i, snapshot) {
                    return React.createElement("li", {
                                key: String(i)
                              }, "Snapshot " + String(i), React.createElement("button", {
                                    onClick: (function (param) {
                                        return gotoSnapshot(snapshot);
                                      })
                                  }, "Restore"));
                  })));
}

var TimeTravelObserver = {
  make: AppSnapshot$TimeTravelObserver
};

function AppSnapshot(Props) {
  var match = Recoil$1.useRecoilState(itemsInCart);
  var setItems = match[1];
  var addItem = function (_e) {
    return setItems(function (prev) {
                return Belt_Array.concat(prev, ["item"]);
              });
  };
  var snapshot = Recoil$1.useRecoilSnapshot();
  console.log("Snapshot", snapshot);
  var logCartItems = Recoil$1.useRecoilCallback(function (param) {
        var snapshot = param.snapshot;
        return function (param) {
          var x = Recoil.Snapshot.get(snapshot, itemsInCart);
          if (typeof x === "number") {
            console.log("loading...");
            return ;
          }
          if (x.TAG === /* HasValue */0) {
            console.log("items", x._0.length);
            return ;
          }
          console.log("error", x._0);
          
        };
      });
  return React.createElement("div", undefined, React.createElement("ol", undefined, Belt_Array.mapWithIndexU(match[0], (function (i, item) {
                        return React.createElement("li", {
                                    key: String(i)
                                  }, item);
                      }))), React.createElement("button", {
                  onClick: (function (e) {
                      return logCartItems(e);
                    })
                }, "Log cart items"), React.createElement("button", {
                  onClick: addItem
                }, "Add item"), React.createElement(AppSnapshot$TimeTravelObserver, {}));
}

var make = AppSnapshot;

export {
  itemsInCart ,
  TimeTravelObserver ,
  make ,
  
}
/* itemsInCart Not a pure module */