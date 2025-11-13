To generate a private DSA key in Node.js, you can use the `crypto` module which is built into Node.js. Below is the complete implementation of the `create_private_key_dsa` function that generates a private DSA key and exports it in PEM format:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  // Generate a DSA key pair
  const dsa = crypto.createDiffieHellman(2048); // 2048 bits is a common choice
  dsa.generateKeys();

  // Export the private key in PEM format
  const privateKeyPem = dsa.getPrivateKey('pem');
  return privateKeyPem;
}
```

This function uses the `crypto` module to generate a DSA key pair with a key size of 2048 bits. It then extracts the private key in PEM format and returns it as a string.