module Styles = {
  open Css;

  let anchor = disabled =>
    style([
      background(`none),
      borderStyle(`none),
      color(`hex(disabled ? "ccc" : "0152cc")),
      cursor(`pointer),
      fontSize(`px(14)),
      padding2(~v=`px(8), ~h=`px(0)),
      pointerEvents(`auto),
      textDecoration(`none),
      pointerEvents(disabled ? `none : `auto),
      hover([background(`hex("0152cc0d"))]),
      active([background(`hex("0152cc19"))]),
    ]);
};

[@react.component]
let make = (~className=?, ~children, ~disabled=false, ~href) => {
  <Next.Link href>
    <a
      className={Clsx.options([|Some(Styles.anchor(disabled)), className|])}>
      children
    </a>
  </Next.Link>;
};