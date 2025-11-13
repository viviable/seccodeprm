To generate a private DSA key in Node.js and export it using the PEM format, you can use the `crypto` module which is built into Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  // Generate a DSA key pair
  const dsa = crypto.createDSA({ size: 2048 });

  // Generate the private key
  dsa.generateKeys();

  // Export the private key in PEM format
  const privateKeyPem = dsa.privateKey.export({
    type: 'pkcs1',
    format: 'pem'
  });

  return privateKeyPem;
}
```

This function uses the `crypto` module to generate a DSA key pair with a key size of 2048 bits. It then exports the private key in PKCS#1 format using the PEM encoding and returns it as a string.