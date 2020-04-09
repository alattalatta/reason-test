open Firebase;

type message =
  | SignIn
  | SignInSuccess(User.t)
  | SignInFailure(Auth.error)
  | SignOut
  | SignOutComplete;

// -- MODEL --
type model = {
  error: option(Auth.error),
  status,
  user: option(User.t),
}
and status =
  | Error
  | Pending
  | SignedIn
  | SignedOut;

let initialValue = {error: None, status: Pending, user: None};

// -- UPDATE --
let%private reducer = (state, action) =>
  switch (action) {
  | SignIn => {...state, status: Pending}
  | SignInSuccess(user) => {error: None, status: SignedIn, user: Some(user)}
  | SignInFailure(error) => {...state, status: Error, error: Some(error)}
  | SignOut => {...state, status: Pending}
  | SignOutComplete => {error: None, status: SignedOut, user: None}
  };

let middleware = (dispatch, state) => {
  let app = App.getInstance((), ());

  let signInWithGoogle =
    Auth.signInWithPopup(Auth.Providers.createGoogleAuthProvider());

  let signIn = () =>
    (app |> App.authInstance |> signInWithGoogle)
    ->Promise.get(payload =>
        switch (payload) {
        | Ok(result) =>
          switch (result.user) {
          | Some(user) => dispatch(SignInSuccess(user))
          | None => dispatch(SignOut)
          }
        | Error(e) => dispatch(SignInFailure(e))
        }
      );

  let signOut = () =>
    (app |> App.authInstance |> Auth.signOut)
    ->Promise.get(() => dispatch(SignOut));

  action => {
    switch (action) {
    | SignIn =>
      if (state.status != Pending) {
        dispatch(SignIn);
      };
      signIn();
    | SignOut =>
      if (state.status != Pending) {
        signOut();
      }
    | _ => dispatch(action)
    };
  };
};

// -- CONTAINER --
let useAuth = () => {
  let (state, dispatch) = React.useReducer(reducer, initialValue);

  React.useEffect0(() => {
    App.getInstance((), ())
    |> App.authInstance
    |> Auth.subscribeAuthState(
         user => {
           switch (user) {
           | Some(user) => dispatch(SignInSuccess(user))
           | None => dispatch(SignOutComplete)
           }
         },
         None,
       )
    |> ignore;
    None;
  });

  (state, middleware(dispatch, state));
};