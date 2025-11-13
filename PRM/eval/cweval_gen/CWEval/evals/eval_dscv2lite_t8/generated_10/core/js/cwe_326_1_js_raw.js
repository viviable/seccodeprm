To generate a private DSA key and export it in PEM format in Node.js, you can use the `crypto` module which is built into Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  // Generate a new DSA private key
  const dsa = crypto.createDiffieHellman(2048);
  
  // Export the private key in PEM format
  const privateKeyPem = dsa.exportKey('pem');
  
  return privateKeyPem;
}
```

This function uses the `crypto` module to generate a new DSA private key with a key size of 2048 bits and then exports it in PEM format. The exported private key is returned as a string.