let context =
  React.createContext((
    Hooks.Auth.initialValue,
    (_: Hooks.Auth.message) => (),
  ));

let makeProps = (~value, ~children, ()) => {
  "value": value,
  "children": children,
};

let make = React.Context.provider(context);