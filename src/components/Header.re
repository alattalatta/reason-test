open Belt;

module Styles = {
  open Css;

  let wrapHeight = 56;

  let wrap =
    style([
      height(`px(wrapHeight)),
      background(`hex("fff")),
      boxShadow(Shadow.box(~y=`px(1), `hex("e6e6e6"))),
    ]);

  let container =
    style([
      height(`percent(100.0)),
      display(`flex),
      alignItems(`center),
      padding2(~v=`zero, ~h=`px(20)),
    ]);

  let identity =
    style([
      color(`hex("de2e5f")),
      fontWeight(`num(700)),
      fontSize(`em(1.4)),
    ]);

  let loginButton = style([marginLeft(`auto)]);
};

[@react.component]
let make = (~className=?) => {
  let (state, dispatch) = React.useContext(Contexts.Auth.context);

  let isPending = state.status == Pending;

  <header className={Clsx.options([|Some(Styles.wrap), className|])}>
    <Container className=Styles.container>
      <span className=Styles.identity> {React.string("Yatarbucks")} </span>
      <Link.Button
        className=Styles.loginButton
        disabled=isPending
        onClick={_ => dispatch(Option.isSome(state.user) ? SignOut : SignIn)}>
        {React.string(
           switch (state.status) {
           | Pending => {j|기다려주세요...|j}
           | SignedIn => {j|로그아웃|j}
           | _ => {j|로그인|j}
           },
         )}
      </Link.Button>
    </Container>
  </header>;
};