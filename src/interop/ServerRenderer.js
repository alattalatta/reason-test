const reprocess = (obj) => JSON.parse(JSON.stringify(obj))

export const fetch = (ssgFn) => async () => {
  const props = await ssgFn()
  return { props: reprocess(props) }
}
