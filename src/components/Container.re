module Styles = {
  open Css;

  let container =
    style([width(`percent(100.0)), maxWidth(`px(1110)), margin(`auto)]);
};

[@react.component]
let make = (~children, ~className=?) =>
  <div className={Clsx.options([|Some(Styles.container), className|])}>
    children
  </div>;