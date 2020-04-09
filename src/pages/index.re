[@react.component]
let make = () => {
  let authStore = Hooks.Auth.useAuth();
  let (myRooms, setMyRooms) = React.useState(() => [||]);

  React.useEffect0(() => {
    setMyRooms(_ => [|{j|안녕|j}, {j|반가워|j}|]);
    None;
  });

  <Contexts.Auth value=authStore>
    <div>
      <Header />
      {if (Array.length(myRooms) > 0) {
         <section>
           <header>
             {React.string({j|내가 참여한 커피타임|j})}
           </header>
           <ul>
             {myRooms
              |> Array.map(room => <strong> {React.string(room)} </strong>)
              |> Array.mapi((idx, el) =>
                   <li key={string_of_int(idx)}> el </li>
                 )
              |> React.array}
           </ul>
         </section>;
       } else {
         React.null;
       }}
      <Next.Link href="/create">
        <a> {React.string({j|새로운 커피타임 추가|j})} </a>
      </Next.Link>
    </div>
  </Contexts.Auth>;
};

let default = make;