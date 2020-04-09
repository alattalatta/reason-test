open Belt;

module Styles = {
  open Css;

  let container =
    style([height(px(56)), display(flexBox), alignItems(center)]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(Contexts.Auth.context);

  let isPending = state.status == Pending;

  <header className=Styles.container>
    <TextButton
      disabled=isPending
      onClick={_ => dispatch(Option.isSome(state.user) ? SignOut : SignIn)}>
      {React.string(
         switch (state.status) {
         | Pending => "..."
         | SignedIn => {j|로그아웃|j}
         | _ => {j|로그인|j}
         },
       )}
    </TextButton>
  </header>;
};