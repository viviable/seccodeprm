To generate a private DSA key in PEM format and return it as a string, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  // Generate a DSA private key
  const dsa = crypto.createDiffieHellman(2048);
  const privateKey = dsa.generateKeys();

  // Export the private key in PEM format
  const pemPrivateKey = dsa.exportKey('pem', {
    type: 'private',
    cipher: 'aes-256-cbc',
    passphrase: 'your-secret-passphrase' // You can set a password if needed
  });

  return pemPrivateKey;
}
```

This function uses the `crypto` module to generate a DSA private key with a key size of 2048 bits. It then exports the private key in PEM format using a specified cipher and passphrase (if desired). The exported private key is returned as a string.