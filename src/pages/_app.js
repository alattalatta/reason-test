import '../global.css';

import { CacheProvider } from '@emotion/core'
import { cache } from 'emotion'
import * as React from 'react'

import * as Contexts_Auth from '../contexts/Contexts_Auth.bs'
import * as Hooks_Auth from '../hooks/Hooks_Auth.bs'

const App = ({ Component, pageProps }) => {
  const authStore = Hooks_Auth.useAuth()

  return (
    <CacheProvider value={cache}>
      <Contexts_Auth.context.Provider value={authStore}>
        <Component {...pageProps} />
      </Contexts_Auth.context.Provider>
    </CacheProvider>
  )
}

export default App