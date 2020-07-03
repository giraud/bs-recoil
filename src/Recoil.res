type stateSetter<'a> = 'a => 'a

module Atom = {
  type t<'a>

  @bs.deriving(abstract)
  type value<'a> = {
    key: string,
    default: 'a,
    @bs.as("persistence_UNSTABLE") @bs.optional
    persistence: string,
  }

  @bs.module("recoil") external make: value<'a> => t<'a> = "atom"

  module Family = {
    @bs.module("recoil") external make: value<'a> => (. 'familyId) => t<'a> = "atomFamily"
  }
}

module Selector = {
  type t<'a>

  module Props = {
    type get = {get: 'a. (. Atom.t<'a>) => 'a}

    type set = {
      get: 'a. (. Atom.t<'a>) => 'a,
      set: 'a. (. Atom.t<'a>, stateSetter<'a>) => unit,
      reset: 'a. (. Atom.t<'a>) => unit,
    }
  }

  @bs.deriving(abstract)
  type value<'a> = {
    key: string,
    get: Props.get => 'a,
    @bs.optional
    set: (Props.set, 'a) => unit,
  }

  module Family = {
    @bs.deriving(abstract)
    type value<'param, 'a> = {
      key: string,
      get: (. 'param) => (. Props.get) => 'a,
      @bs.optional
      set: (. 'param) => (. Props.set, 'a) => unit,
    }

    @bs.module("recoil") external make: value<'param, 'a> => (. 'param) => Atom.t<'a> = "selectorFamily"
  }

  @bs.module("recoil") external make: value<'a> => Atom.t<'a> = "selector"
}

module Loadable = {
  type t<'a, 'error>

  type state<'a, 'error> =
    | HasValue('a)
    | Loading
    | Error('error)

  let classify: t<'a, 'error> => state<'a, 'error> =
    loadable => {
      let state = Obj.magic(loadable)["state"]
      switch (state) {
      | "hasValue" => HasValue(Obj.magic(loadable)["contents"])
      | "loading" => Loading
      | _ => Error(Obj.magic(loadable)["contents"])
      }
    }
}

module Snapshot = {
  type t

  @bs.send external getLoadable: (t, Atom.t<'a>) => Loadable.t<'a, 'error> = "getLoadable"

  let get = (snap, atom) => snap->getLoadable(atom)->Loadable.classify
}

module Callback = {
  type t = {
    snapshot: Snapshot.t,
    gotoSnapshot: Snapshot.t => unit,
  }

  type fn<'params, 'result> = (. 'params) => 'result
}

/**
 Returns true if value is either an atom or selector and false otherwise.
 */
@bs.module("recoil")
external isRecoilValue: 'a => bool = "isRecoilValue"

/**
 Returns a tuple where the first element is the value of state and
 the second element is a setter function that will update the value of the given state when called.

 This API is similar to the React useState() hook except
 it takes a Recoil state instead of a default value as an argument.
 */
@bs.module("recoil")
external useState: Atom.t<'a> => ('a, (. stateSetter<'a>) => unit) = "useRecoilState"

/**
 Returns the value of the given Recoil state.

 This is the recommended hook to use when a component intends to read state without writing to it,
 as this hook works with both read-only state and writeable state.
 */
@bs.module("recoil")
external useValue: Atom.t<'a> => 'a = "useRecoilValue"

/**
 Returns a setter function for updating the value of writeable Recoil state.

 This is the recommended hook to use when a component intends to write to state without reading it.
 */
@bs.module("recoil")
external useSetState: Atom.t<'a> => (. stateSetter<'a>) => unit = "useSetRecoilState"

/**
 Returns a function that will reset the value of the given state to its default value.
 */
@bs.module("recoil")
external useResetState: Atom.t<'a> => (. unit) => unit = "useResetRecoilState"

/**
 See useCallback.
 */
@bs.module("recoil")
external useCallback0: (Callback.t => (. 'params) => 'result) => Callback.fn<'params, 'result> = "useRecoilCallback"

/**
 This hook is similar to useCallback(), but will also provide an API for your callbacks to work with Recoil state.
 */
@bs.module("recoil")
external useCallback: (Snapshot.t => (. 'params) => 'result, array<string>) => Callback.fn<'params, 'result> =
  "useRecoilCallback"

/**
 This hook synchronously returns a Snapshot object during rendering and
 subscribes the calling component for all Recoil state changes.
 */
@bs.module("recoil")
external useSnapshot: unit => Snapshot.t = "useRecoilSnapshot"

/**
 This hook returns a callback which takes a Snapshot as a parameter and
 will update the current <RecoilRoot> state to match this atom state.
 */
@bs.module("recoil")
external useGotoSnapshot: unit => (. Snapshot.t) => unit = "useGotoRecoilSnapshot"

type transactionObserverCb = {
  snapshot: Snapshot.t,
  previousSnapshot: Snapshot.t,
}

/**
 This hook subscribes a callback to be executed every time there is a change to Recoil atom state.
 Multiple updates may be batched together in a single transaction.
 */
@bs.module("recoil")
external useTransactionObserver: ((. transactionObserverCb) => unit) => unit = "useRecoilTransactionObserver_UNSTABLE"

module Root = Recoil_Root
module Observer = Recoil_Observer
module Logger = Recoil_Logger