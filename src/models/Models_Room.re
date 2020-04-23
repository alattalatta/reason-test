module Type = {
  type menu = {
    count: int, // 같은 메뉴를 선택한 수
    from: array(string), // 넣은 사람 이름
    name: string, // 메뉴 이름
    options: string // 메뉴 옵션
  };

  type room = {
    available: bool, // 선택 가능 여부
    public: bool, // 공개 여부
    // -- collections --
    c_selections: string,
  };
};

module Decode = {
  open Type;

  let menu = v =>
    Json.Decode.{
      count: field("count", int, v),
      from: field("from", array(string), v),
      name: field("name", string, v),
      options: field("options", string, v),
    };

  let room = v =>
    Json.Decode.{
      available: field("available", bool, v),
      public: field("public", bool, v),
      c_selections: "selections",
    };
};

module Encode = {
  open Type;

  let menu = v =>
    Json.Encode.(
      object_([
        ("count", int(v.count)),
        ("from", array(string, v.from)),
        ("name", string(v.name)),
        ("options", string(v.options)),
      ])
    );

  let room = v =>
    Json.Encode.(
      object_([
        ("available", bool(v.available)),
        ("public", bool(v.public)),
      ])
    );
};