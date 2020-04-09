%bs.raw
{|import 'isomorphic-unfetch'|};

let%private bsPromiseToReason = promise =>
  promise->Promise.Js.fromBsPromise->Promise.Js.toResult;

let fetch = url => Fetch.fetch(url)->bsPromiseToReason;

let fetchWithInit = (url, init) =>
  Fetch.fetchWithInit(url, init)->bsPromiseToReason;

module Response = {
  let json = response => response->Fetch.Response.json->bsPromiseToReason;
  let text = response => response->Fetch.Response.text->bsPromiseToReason;
};