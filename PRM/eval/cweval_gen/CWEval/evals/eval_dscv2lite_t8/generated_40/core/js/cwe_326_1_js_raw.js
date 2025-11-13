To generate a private DSA key and export it in PEM format in JavaScript, you can use the `crypto` module provided by Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  // Generate a new DSA key pair
  const dsa = crypto.createDiffieHellman(2048, 'sha256');
  const privateKeyPEM = dsa.getPrivateKey('pem');

  return privateKeyPEM;
}
```

This function uses the `crypto` module to generate a new DSA key pair with a key size of 2048 bits and SHA-256 as the hash algorithm. It then extracts the private key in PEM format and returns it as a string.