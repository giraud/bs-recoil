ReactDOMRe.renderToElementWithId(
  <Recoil.Root>
    <Recoil.Logger collapsed=false />
    <h1> "Todo"->React.string </h1>
    <AppTodoList />
    <h1> "Family"->React.string </h1>
    <AppFamily />
    <h1> "Persistence"->React.string </h1>
    "Open the console to see messages"->React.string
    <AppPersistence />
    <h1> "Snapshot"->React.string </h1>
    <AppSnapshot />
  </Recoil.Root>,
  "app",
);