module Styles = {
  open Css;

  let header =
    style([position(`fixed), top(`zero), right(`zero), left(`zero)]);

  let body = style([paddingTop(`px(Header.Styles.wrapHeight))]);
};

[@react.component]
let make = (~children) =>
  <Container>
    <Header className=Styles.header />
    <div className=Styles.body> children </div>
  </Container>;