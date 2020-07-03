/**
 Provides the context in which atoms have values.

 Must be an ancestor of any component that uses any Recoil hooks.
 Multiple roots may co-exist; atoms will have distinct values within each root.
 If they are nested, the innermost root will completely mask any outer roots.
 */
@bs.module("recoil") @react.component
external make: (~children: React.element) => React.element = "RecoilRoot";