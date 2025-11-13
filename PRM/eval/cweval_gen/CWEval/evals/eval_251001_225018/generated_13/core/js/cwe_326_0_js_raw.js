To generate an RSA private key and export it in PEM format in JavaScript, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
  const keySize = 2048; // Key size in bits
  const rsaKeyPair = crypto.generateKeyPairSync('rsa', {
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

  return rsaKeyPair.privateKey;
}
```

This function uses the `crypto` module to generate an RSA key pair with a specified key size (2048 bits in this case) and then returns the private key in PEM format.