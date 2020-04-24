open Belt;
open Firebase;

[@react.component]
let make = () => {
  let authStore = Hooks.Auth.useAuth();
  let (myRooms, setMyRooms) = React.useState(() => [||]);

  React.useEffect0(() => {
    setMyRooms(_ => [|{j|안녕|j}, {j|반가워|j}|]);
    None;
  });

  React.useEffect0(() => {
    App.getInstance((), ())
    ->App.firestoreInstance
    ->Firestore.query("time")
    ->Firestore.Query.get
    ->Promise.mapOk(a => a.docs)
    ->Promise.mapOk(docs =>
        Array.map(
          docs,
          snap => {
            let data = Firestore.Document.QuerySnapshot.getData(snap);
            try(Some(Models.Time.Decode.data(data))) {
            | Json.Decode.DecodeError(_) => None
            };
          },
        )
        ->Array.keepMap(x => x)
      )
    ->Promise.tapOk(Js.Console.log)
    ->Promise.tapError(Js.Console.error)
    ->ignore;
    None;
  });

  let addSome = _ =>
    App.getInstance((), ())
    ->App.firestoreInstance
    ->Firestore.collection("time")
    ->Firestore.Collection.doc("advent")
    ->Firestore.Document.set(Models.Time.Encode.data({ticking: "good!"}))
    ->Promise.tapOk(() =>
        Webapi.Dom.Window.alert("This is madness!", Webapi.Dom.window)
      )
    ->ignore;

  <Contexts.Auth value=authStore>
    <Layout>
      {if (Array.length(myRooms) > 0) {
         <Card>
           <section>
             <Heading>
               {React.string({j|내가 주문한 커피타임|j})}
             </Heading>
             <ul>
               {myRooms
                ->Array.map(room => React.string(room))
                ->Array.mapWithIndex((idx, el) =>
                    <li key={string_of_int(idx)}> el </li>
                  )
                ->React.array}
             </ul>
             <TextButton onClick=addSome>
               {React.string({j|어디 하나 던져보아라|j})}
             </TextButton>
           </section>
         </Card>;
       } else {
         React.null;
       }}
      <Next.Link href="/create">
        <a> {React.string({j|새로운 커피타임 추가|j})} </a>
      </Next.Link>
    </Layout>
  </Contexts.Auth>;
};

let default = make;