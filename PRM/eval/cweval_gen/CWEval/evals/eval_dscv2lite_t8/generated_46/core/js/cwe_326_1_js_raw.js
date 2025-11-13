To generate a private DSA key and export it in PEM format in JavaScript, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
    // Generate a DSA private key
    const dsaKeyPair = crypto.generateKeyPairSync('dsa', {
        modulusLength: 2048,
        divisorLength: 256
    });

    // Export the private key in PEM format
    const privateKeyPEM = dsaKeyPair.privateKey.export({
        type: 'pkcs1',
        format: 'pem'
    });

    return privateKeyPEM;
}
```

This function uses the `crypto` module to generate a DSA key pair with a modulus length of 2048 bits and a divisor length of 256 bits. It then exports the private key in PKCS#1 format and returns it as a PEM string.