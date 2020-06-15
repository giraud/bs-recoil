ReactDOMRe.renderToElementWithId(
  <React.StrictMode>
    <Recoil.Root>
      <Recoil.Logger collapsed=false />
      <h1> "Todo"->React.string </h1>
      <AppTodoList />
      <h1> "Family"->React.string </h1>
      <AppFamily />
    </Recoil.Root>
  </React.StrictMode>,
  "app",
);