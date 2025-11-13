To generate an RSA private key in PEM format, we can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function that generates an RSA private key and returns it as a PEM formatted string:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
    const keySize = 2048; // Key size in bits
    const rsaKeyPair = crypto.generateKeyPairSync('rsa', {
        modulusLength: keySize,
        publicKeyEncoding: {
            type: 'spki',
            format: 'pem'
        },
        privateKeyEncoding: {
            type: 'pkcs8',
            format: 'pem'
        }
    });
    return rsaKeyPair.privateKey;
}
```

This function uses the `crypto.generateKeyPairSync` method to generate an RSA key pair synchronously. It specifies the key size, encoding formats for both public and private keys, and then returns the private key in PEM format.