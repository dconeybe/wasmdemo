import resolve from '@rollup/plugin-node-resolve';
import commonjs from '@rollup/plugin-commonjs';
import typescriptPlugin from '@rollup/plugin-typescript';
import json from '@rollup/plugin-json';

export default {
  input: 'src/index_browser.ts',
  output: {
    file: 'dist/bundle.browser.js',
    format: 'iife',
    sourcemap: true
  },
  plugins: [resolve(), typescriptPlugin(), commonjs(),  json()
  ]
};
