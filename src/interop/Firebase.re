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
  type unsubscribe = unit => unit;
  type _userCredentials = {user: Js.nullable(User.t)};
  type userCredentials = {user: option(User.t)};

  [@bs.send]
  external _onAuthStateChanged:
    (
      t,
      ~success: Js.nullable(User.t) => unit,
      ~error: error => unit=?,
      unit
    ) =>
    unsubscribe =
    "onAuthStateChanged";

  let subscribeAuthState = (success, error, auth) => {
    let onSuccess = user => Js.Nullable.toOption(user) |> success;

    switch (error) {
    | Some(onError) =>
      _onAuthStateChanged(auth, ~success=onSuccess, ~error=onError, ())
    | None => _onAuthStateChanged(auth, ~success=onSuccess, ())
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

module Firestore = {
  type t;

  type collectionRef
  and docRef;

  type querySnapshot = {
    docs: array(documentSnapshot),
    size: int,
    empty: bool,
  }
  and documentSnapshot = {
    id: string,
    exists: bool,
    ref: docRef,
  };

  module Collection = {
    type t = collectionRef;

    [@bs.send] external doc: (t, string) => docRef = "doc";

    [@bs.send] external get: t => Promise.t(querySnapshot) = "get";

    [@bs.send]
    external onSnapshot:
      (t, querySnapshot => unit, ~error: Js.Exn.t => unit=?, unit) => unit =
      "onSnapshot";

    // queries
    [@bs.send]
    external orderBy: (t, string, [@bs.string] [ | `desc | `asc]) => t =
      "orderBy";

    [@bs.send] external limit: (t, int) => t = "limit";

    [@bs.send] external startAt: (t, docRef) => t = "startAt";
    [@bs.send] external startAfter: (t, docRef) => t = "startAfter";

    [@bs.send] external endBefore: (t, docRef) => t = "endBefore";
    [@bs.send] external endAt: (t, docRef) => t = "endAt";
  };

  module Doc = {
    type t = docRef;

    [@bs.send] external collection: (t, string) => Collection.t = "collection";

    [@bs.send] [@bs.return nullable]
    external getData: documentSnapshot => option(Js.Json.t) = "getData";

    [@bs.send]
    external onSnapshot:
      (t, documentSnapshot => unit, ~error: Js.Exn.t => unit=?, unit) => unit =
      "onSnapshot";
  };

  [@bs.send] external collection: (t, string) => Collection.t = "collection";
  [@bs.send] external doc: (t, string) => Doc.t = "doc";

  // CRUD
  [@bs.send]
  external get: t => Promise.Js.t(documentSnapshot, Js.Exn.t) = "get";
  [@bs.send] external delete: t => Promise.Js.t(unit, Js.Exn.t) = "delete";
  [@bs.send] external set: ('a, t) => Promise.Js.t(unit, Js.Exn.t) = "set";
  [@bs.send]
  external update: ('a, t) => Promise.Js.t(unit, Js.Exn.t) = "update";
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
  [@bs.send] external firestoreInstance: t => Firestore.t = "firestore";

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