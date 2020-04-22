open Belt;

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

  type collectionReference;

  type documentReference;

  type documentSnapshot = {
    id: string,
    exists: bool,
    ref: documentReference,
  };

  type queryDocumentSnapshot = {
    id: string,
    ref: documentReference,
  };

  type query;

  type querySnapshot = {
    docs: array(queryDocumentSnapshot),
    size: int,
    empty: bool,
  };

  module Query = {
    type t = query;

    [@bs.send] external _get: t => Promise.t(querySnapshot) = "get";
    let get = query => _get(query)->Promise.Js.toResult;

    [@bs.send]
    external onSnapshot:
      (t, querySnapshot => unit, ~error: Js.Exn.t => unit=?, unit) => unit =
      "onSnapshot";

    // queries
    [@bs.send]
    external orderBy: (t, string, [@bs.string] [ | `desc | `asc]) => t =
      "orderBy";

    [@bs.send] external limit: (t, int) => t = "limit";

    [@bs.send] external startAt: (t, documentReference) => t = "startAt";
    [@bs.send] external startAfter: (t, documentReference) => t = "startAfter";

    [@bs.send] external endBefore: (t, documentReference) => t = "endBefore";
    [@bs.send] external endAt: (t, documentReference) => t = "endAt";

    [@bs.send]
    external where:
      (
        t,
        string,
        [@bs.string] [
          | [@bs.as "<"] `lt
          | [@bs.as "<="] `lte
          | [@bs.as "=="] `eq
          | [@bs.as ">="] `gte
          | [@bs.as ">"] `gt
        ],
        'a
      ) =>
      t =
      "where";

    module Snapshot = {
      type t = querySnapshot;

      [@bs.get] external getQuery: t => query = "query";

      [@bs.get] external size: t => int = "size";

      [@bs.get] external isEmpty: t => bool = "empty";
    };
  };

  module Reference = {
    module type ReferenceBase = {type t;};

    module Make = (M: ReferenceBase) => {
      type t = M.t;
      [@bs.get] external getID: M.t => string = "id";
      [@bs.get] external getPath: M.t => string = "path";
    };
  };

  module Collection = {
    include Reference.Make({
      type t = collectionReference;
    });

    [@bs.get] [@bs.return nullable]
    external getParent: t => option(documentReference) = "parent";

    [@bs.send]
    external _add: (t, Js.Json.t) => Promise.Js.t(documentReference, Js.Exn.t) =
      "add";
    let add = (collection, data) =>
      _add(collection, data)->Promise.Js.toResult;

    [@bs.send] external doc: (t, string) => documentReference = "doc";

    external asQuery: t => Query.t = "%identity";
  };

  module Document = {
    include Reference.Make({
      type t = documentReference;
    });

    [@bs.get] external getParent: t => collectionReference = "parent";

    [@bs.send] external collection: (t, string) => Collection.t = "collection";

    [@bs.send]
    external onSnapshot:
      (t, documentSnapshot => unit, ~error: Js.Exn.t => unit=?, unit) => unit =
      "onSnapshot";

    // CRUD
    [@bs.send]
    external _get: t => Promise.Js.t(documentSnapshot, Js.Exn.t) = "get";
    let get = document => _get(document)->Promise.Js.toResult;

    /**
   * Writes to the document referred to by this `DocumentReference`. If the
   * document does not yet exist, it will be created. If you pass
   * `SetOptions`, the provided data can be merged into an existing document.
   *
   * * `data` A map of the fields and values for the document.
   * * `options` An object to configure the set behavior.
   *
   * Returns a Promise resolved once the data has been successfully written
   * to the backend (Note that it won't resolve while you're offline).
   */
    [@bs.send]
    external _set: (t, 'a) => Promise.Js.t(unit, Js.Exn.t) = "set";
    let set = (document, data) => _set(document, data)->Promise.Js.toResult;

    /**
   * Updates fields in the document referred to by this `DocumentReference`.
   * The update will fail if applied to a document that does not exist.
   *
   * * `data` An object containing the fields and values with which to
   * update the document. Fields can contain dots to reference nested fields
   * within the document.
   *
   * Return a Promise resolved once the data has been successfully written
   * to the backend (Note that it won't resolve while you're offline).
   */
    [@bs.send]
    external _update: (t, 'a) => Promise.Js.t(unit, Js.Exn.t) = "update";
    let update = (document, data) =>
      _update(document, data)->Promise.Js.toResult;

    [@bs.send] external _delete: t => Promise.Js.t(unit, Js.Exn.t) = "delete";
    let delete = document => _delete(document)->Promise.Js.toResult;

    module Snapshot = {
      type t = documentSnapshot;

      [@bs.send] [@bs.return nullable]
      external getData: t => option(Js.Json.t) = "data";
    };

    module QuerySnapshot = {
      type t = queryDocumentSnapshot;

      [@bs.send] external getData: t => Js.Json.t = "data";
    };
  };

  [@bs.send] external collection: (t, string) => Collection.t = "collection";

  [@bs.send] external doc: (t, string) => Document.t = "doc";

  [@bs.send] external query: (t, string) => Query.t = "collection";
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
    apps[0]
    ->Option.map((app, ()) => app) // thunk for lazyness
    ->Option.getWithDefault(() =>
        _initializeApp({
          apiKey: "AIzaSyCKLD787gEI3PE5wcuLQTU92eDB1GYiedk",
          authDomain: "yatarbucks.firebaseapp.com",
          projectId: "yatarbucks",
        })
      );
};