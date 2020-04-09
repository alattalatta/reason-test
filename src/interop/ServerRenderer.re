type fetcher('a) = Next.context => 'a;

[@bs.module "./serverRenderer"] external fetch: fetcher('a) => 'a = "fetch";