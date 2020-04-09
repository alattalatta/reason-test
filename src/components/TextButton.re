module Styles = {
  open Css;

  let button =
    style([
      background(none),
      borderStyle(none),
      color(hex("0152cc")),
      cursor(pointer),
      fontSize(px(14)),
      padding2(~v=px(8), ~h=px(0)),
      hover([background(hex("0152cc0d"))]),
      active([background(hex("0152cc19"))]),
      disabled([color(hex("ccc")), pointerEvents(none)]),
    ]);
};

[@react.component]
let make = (~className=?, ~children, ~disabled=?, ~onClick) => {
  <button
    className={Clsx.options([|Some(Styles.button), className|])}
    disabled={Belt.Option.getWithDefault(disabled, false)}
    onClick>
    children
  </button>;
};