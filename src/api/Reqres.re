module Type = {
  type data = {
    id: int,
    email: string,
    first_name: string,
    last_name: string,
    avatar: string,
  };

  type ad = {
    company: string,
    url: string,
    text: string,
  };

  type user = {
    data,
    ad,
  };
};

module Decode = {
  open Type;

  let data = v =>
    Json.Decode.{
      id: v |> field("id", int),
      email: v |> field("email", string),
      first_name: v |> field("first_name", string),
      last_name: v |> field("last_name", string),
      avatar: v |> field("avatar", string),
    };

  let ad = v =>
    Json.Decode.{
      company: v |> field("company", string),
      url: v |> field("url", string),
      text: v |> field("text", string),
    };

  let user = v =>
    Json.Decode.{data: v |> field("data", data), ad: v |> field("ad", ad)};
} /* }*/;

module Request = {
  let singleUser = id => {
    Unfetch.fetch("https://reqres.in/api/users/" ++ id)
    ->Promise.flatMapOk(Unfetch.Response.json)
    ->Promise.map(v =>
        switch (v) {
        | Ok(s) =>
          try(Ok(Decode.user(s))) {
          | Json.Decode.DecodeError(s) => Error(s)
          }
        | Error(_) => Error("fetch error")
        }
      );
  };
};