import resolve from '@rollup/plugin-node-resolve';
import commonjs from '@rollup/plugin-commonjs';
import typescript from 'rollup-plugin-typescript2';

const typescriptOptions = {
  tsconfigOverride: {
    exclude: ['**/*_node.ts']
  }
};

export default {
  input: 'src/index_browser.ts',
  output: {
    file: 'dist/bundle.browser.js',
    format: 'iife',
    sourcemap: true
  },
  plugins: [resolve(), typescript(typescriptOptions), commonjs()]
};
