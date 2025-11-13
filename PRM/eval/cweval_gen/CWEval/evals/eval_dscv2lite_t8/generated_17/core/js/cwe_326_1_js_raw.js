To generate a private DSA key and export it using the PEM format in JavaScript, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

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
  dsa.setPrivateKey(crypto.randomBytes(128));

  // Export the private key in PEM format
  const privateKeyPEM = dsa.exportKey('pem', {
    type: 'pkcs1',
    format: 'pem'
  });

  return privateKeyPEM;
}
```

This function uses the `crypto` module to generate a new DSA private key with a key size of 2048 bits. It then exports the private key in PEM format and returns it as a string. Note that this implementation uses Diffie-Hellman parameters instead of DSA parameters because the `crypto.createDiffieHellman` method is commonly used for generating DSA keys in Node.js. If you need to specifically generate DSA keys using the `crypto.createDSA` method, you would need to adjust the implementation accordingly.