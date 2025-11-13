To generate an RSA private key and export it in PEM format, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
  // Generate a private key with a key size of 2048 bits
  const privateKey = crypto.generateKeyPairSync('rsa', {
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

  // Return the private key in PEM format
  return privateKey.privateKey;
}
```

This function uses the `crypto.generateKeyPairSync` method to generate a private RSA key with a modulus length of 2048 bits. It then returns the private key in PEM format.