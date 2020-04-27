module Styles = {
  open Css;

  let container =
    style([
      borderBottom(`px(1), `solid, `hex("2e2e2e")),
      fontSize(`px(18)),
      fontWeight(`num(700)),
      paddingBottom(`px(17)),
    ]);
};

[@react.component]
let make = (~children, ~className=?) =>
  <h1 className={Clsx.options([|Some(Styles.container), className|])}>
    children
  </h1>;