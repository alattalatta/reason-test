// const withAnalyzer = require('@next/bundle-analyzer')
const withTM = require('next-transpile-modules')(['@glennsl/bs-json', 'bs-css', 'bs-css-emotion', 'bs-platform', 'reason-promise'])

module.exports =
  withTM({
    pageExtensions: ['jsx', 'js', 'bs.js'],
  })
