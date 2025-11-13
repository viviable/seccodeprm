To generate an RSA private key and export it in PEM format using JavaScript, you can utilize the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
    // Generate a 2048-bit RSA private key
    const privateKey = crypto.generateKeyPairSync('rsa', {
        modulusLength: 2048,
        publicKeyEncoding: {
            type: 'spki',
            format: 'pem'
        },
        privateKeyEncoding: {
            type: 'pkcs8',
            format: 'pem'
        }
    });

    return privateKey.privateKey;
}
```

This function uses the `crypto` module to generate a synchronous RSA key pair with a modulus length of 2048 bits. The private key is then exported in PKCS#8 format and returned as a PEM string.