To generate a private DSA key and export it in PEM format, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
    const dsa = crypto.createDiffieHellman(2048);
    const privateKeyPEM = dsa.exportKey('pem');
    return privateKeyPEM;
}
```

This function uses the `crypto` module to create a Diffie-Hellman instance with a 2048-bit key size. The `exportKey` method is then used to export the private key in PEM format.