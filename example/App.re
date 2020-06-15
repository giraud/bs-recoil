ReactDOMRe.renderToElementWithId(
  <Recoil.Root>
    <h1> "Todo"->React.string </h1>
    <AppTodoList />
    <h1> "Family"->React.string </h1>
    <AppFamily />
  </Recoil.Root>,
  "app",
);