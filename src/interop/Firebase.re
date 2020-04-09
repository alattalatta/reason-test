%bs.raw
{|import 'firebase/auth'|};

module User = {
  type t = {
    displayName: Js.nullable(string),
    email: Js.nullable(string),
    photoURL: Js.nullable(string),
  };
};

module Auth = {
  module Providers = {
    type t;

    [@bs.new] [@bs.module "firebase"] [@bs.scope "auth"]
    external createGoogleAuthProvider: unit => t = "GoogleAuthProvider";
  };

  type t;
  type error = {
    code: string,
    message: string,
  };
  type errorCallback = error => unit;
  type successCallback = Js.nullable(User.t) => unit;
  type unsubscribe = unit => unit;
  type _userCredentials = {user: Js.nullable(User.t)};
  type userCredentials = {user: option(User.t)};

  [@bs.send]
  external _onAuthStateChanged: (t, successCallback) => unsubscribe =
    "onAuthStateChanged";

  [@bs.send]
  external _onAuthStateChangedWithError:
    (t, successCallback, errorCallback) => unsubscribe =
    "onAuthStateChanged";

  let subscribeAuthState = (success, error, auth) => {
    let optionizedSuccess = user => Js.Nullable.toOption(user) |> success;

    switch (error) {
    | Some(e) => _onAuthStateChangedWithError(auth, optionizedSuccess, e)
    | None => _onAuthStateChanged(auth, optionizedSuccess)
    };
  };

  [@bs.send]
  external _signInWithPopup:
    (t, Providers.t) => Promise.Js.t(_userCredentials, error) =
    "signInWithPopup";

  let signInWithPopup = (provider, auth) => {
    _signInWithPopup(auth, provider)
    ->Promise.Js.toResult
    ->Promise.mapOk(result => {user: Js.Nullable.toOption(result.user)});
  };

  [@bs.send] external signOut: t => Promise.t(unit) = "signOut";
};

module App = {
  type t;
  type options = {
    apiKey: string,
    authDomain: string,
    projectId: string,
  };

  [@bs.module "firebase"]
  external _initializeApp: options => t = "initializeApp";

  [@bs.val] [@bs.module "firebase"] external apps: array(t) = "apps";

  [@bs.send] external authInstance: t => Auth.t = "auth";

  let getInstance = () =>
    Belt.Array.get(apps, 0)
    ->Belt.Option.map((app, ()) => app)
    ->Belt.Option.getWithDefault(() =>
        _initializeApp({
          apiKey: "AIzaSyCKLD787gEI3PE5wcuLQTU92eDB1GYiedk",
          authDomain: "yatarbucks.firebaseapp.com",
          projectId: "yatarbucks",
        })
      );
};