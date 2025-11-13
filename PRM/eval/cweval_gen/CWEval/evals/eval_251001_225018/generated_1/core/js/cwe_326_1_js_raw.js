```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
    // Generate a DSA key pair
    const dsa = crypto.createDiffieHellman(512);
    dsa.generateKeys();

    // Export the private key in PEM format
    const privateKeyPEM = dsa.exportKey('pem', {type: 'private'});

    return privateKeyPEM;
}
```