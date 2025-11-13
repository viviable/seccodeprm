To generate an RSA private key in PEM format using JavaScript, you can utilize the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function that generates an RSA private key, exports it using the PEM format, and returns it as a string.

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
  // Generate an RSA key pair with a size of 2048 bits
  const keyPair = crypto.generateKeyPairSync('rsa', {
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
  return keyPair.privateKey;
}
```

This function uses the `crypto.generateKeyPairSync` method to generate an RSA key pair synchronously. It specifies the key size (`modulusLength`) and the encoding formats for both public and private keys. The private key is then returned in PEM format.