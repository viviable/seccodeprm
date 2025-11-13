To generate a private DSA key and export it using the PEM format in JavaScript, you can use the `crypto` module provided by Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
    const keyPair = crypto.generateKeyPairSync('dsa', {
        modulusLength: 2048,
        divisorLength: 256,
    });

    return keyPair.privateKey.export({
        type: 'pkcs1',
        format: 'pem'
    });
}
```

This function uses the `crypto` module to generate a synchronous DSA key pair with a modulus length of 2048 bits and a divisor length of 256 bits. It then exports the private key in PKCS#1 PEM format and returns it as a string.