module Styles = {
  open Css;

  let container =
    style([
      backgroundColor(`hex("fff")),
      borderRadius(`px(4)),
      boxShadow(Shadow.box(~y=`px(1), `hex("e6e6e6"))),
      padding(`px(20)),
    ]);
};

[@react.component]
let make = (~children, ~className=?) =>
  <div className={Clsx.options([|Some(Styles.container), className|])}>
    children
  </div>;