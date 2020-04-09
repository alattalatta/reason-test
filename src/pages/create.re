type props = {rooms: array(int)};

[@react.component]
let make = () => {
  let (title, setTitle) = React.useState(() => "");

  <div>
    <h1> {React.string(title)} </h1>
    <form>
      <input
        value=title
        onChange={event =>
          setTitle(_ => ReactEvent.Form.target(event)##value)
        }
      />
    </form>
  </div>;
};

let default = make;