// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Recoil = require("recoil");

function formatDate(date) {
  return date.getHours().toString() + ":" + date.getMinutes().toString() + ":" + date.getSeconds().toString();
}

function Recoil_Logger(Props) {
  var collapsedOpt = Props.collapsed;
  var collapsed = collapsedOpt !== undefined ? collapsedOpt : true;
  Recoil.useTransactionObservation_UNSTABLE(function (param) {
        var previousAtomValues = param.previousAtomValues;
        var atomValues = param.atomValues;
        param.modifiedAtoms.forEach(function (name) {
              var consoleGroup = collapsed ? console.groupCollapsed : console.group;
              consoleGroup(formatDate(new Date()) + " " + name);
              console.log("Previous:", previousAtomValues.get(name));
              console.log("New:", atomValues.get(name));
              console.groupEnd();
              
            });
        
      });
  return null;
}

var $$Set;

var $$Map;

var make = Recoil_Logger;

exports.$$Set = $$Set;
exports.$$Map = $$Map;
exports.formatDate = formatDate;
exports.make = make;
/* recoil Not a pure module */
