To generate an RSA private key in PEM format using JavaScript, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
  const keySize = 2048; // Key size in bits
  const privateKey = crypto.generateKeyPairSync('rsa', {
    modulusLength: keySize,
    publicKeyEncoding: {
      type: 'spki',
      format: 'pem'
    },
    privateKeyEncoding: {
      type: 'pkcs8',
      format: 'pem'
    }
  });

  return privateKey.privateKey;
}
```

This function uses the `crypto.generateKeyPairSync` method to generate an RSA key pair synchronously. It specifies the key size and encodings for both the public and private keys. The private key is then returned in PEM format as a string.