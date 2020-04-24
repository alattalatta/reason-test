open Belt;

module Styles = {
  open Css;

  let wrap = style([height(`px(56))]);

  let container = style([display(`flex), alignItems(`center)]);

  let loginButton = style([marginLeft(`auto)]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(Contexts.Auth.context);

  let isPending = state.status == Pending;

  <header className=Styles.wrap>
    <Container className=Styles.container>
      <TextButton
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
      </TextButton>
    </Container>
  </header>;
};