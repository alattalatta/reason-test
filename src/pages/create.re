type props = {rooms: array(int)};

[@react.component]
let make = () => {
  let (title, setTitle) = React.useState(() => "");

  <Layout>
    <h1> {React.string(title)} </h1>
    <form>
      <input
        value=title
        onChange={event => ReactEvent.Form.target(event)##value->setTitle}
      />
    </form>
  </Layout>;
};

let default = make;