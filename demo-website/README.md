# Common Setup for Browser

1. Run build on the parent directory
2. Cd into this demo-website folder, and copy/replace build/www/index.mjs from parent to src folder.
3. Run `npm install` to install dependencies.
4. Run `npm run build` to generate the compiled JavaScript.
5. Open `index.html` in a web browser, and click the "Run Test" button.


# Using your own v9 SDK:

```
cd ~/firebase-js-sdk/packages/firestore
yarn build:debug
yarn pack  # this resulted in a tar file (e.g. firebase-firestore-v3.4.4.tgz)
cd ~/firestore-team/v9web
npm install firebase-firestore-v3.4.4.tgz
```
