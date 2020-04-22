module Type = {
  type data = {ticking: string};
};

module Decode = {
  open Type;

  let data = v => Json.Decode.{ticking: v |> field("somewhat", string)};
};

module Encode = {
  open Type;

  let data = v => Json.Encode.(object_([("somewhat", string(v.ticking))]));
};