To generate an RSA private key in PEM format using JavaScript, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
  // Generate a new RSA private key with a key size of 2048 bits
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

  return privateKey.privateKey;
}
```

This function uses the `crypto` module to generate a new RSA key pair synchronously. It specifies a modulus length of 2048 bits for the key. The public key is encoded in SPKI format and the private key is encoded in PKCS#8 format, both in PEM format. Finally, it returns the private key as a string.