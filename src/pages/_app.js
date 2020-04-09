import '../global.css';

import { CacheProvider } from '@emotion/core'
import { cache } from 'emotion'
import NextApp from 'next/app'
import * as React from 'react'

export default class App extends NextApp {
  render() {
    const { Component, pageProps } = this.props
    return (
      <CacheProvider value={cache}>
        <Component {...pageProps} />
      </CacheProvider>
    )
  }
}