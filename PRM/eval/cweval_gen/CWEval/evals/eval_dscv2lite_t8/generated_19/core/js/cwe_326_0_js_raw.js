To generate an RSA private key in PEM format using JavaScript, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
  const key = crypto.generateKeyPairSync('rsa', {
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

  return key.privateKey;
}
```

This function uses Node.js's built-in `crypto` module to generate an RSA key pair synchronously. It specifies the key length and encodings for both the public and private keys. The private key is then returned in PEM format as a string.