To generate an RSA private key and export it in PEM format in JavaScript, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
  const { privateKey } = crypto.generateKeyPairSync('rsa', {
    modulusLength: 2048,
    publicKeyEncoding: {
      type: 'spki',
      format: 'pem'
    },
    privateKeyEncoding: {
      type: 'pkcs8',
      format: 'pem'
    }
  });
  return privateKey;
}
```

This function uses the `crypto.generateKeyPairSync` method to generate an RSA key pair with a modulus length of 2048 bits. It then returns the private key in PEM format. Note that this function must be run in a Node.js environment where the `crypto` module is available.