To generate an RSA private key and export it in PEM format, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
    // Generate an RSA private key with a 2048-bit key length
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

This function uses the `crypto.generateKeyPairSync` method to generate an RSA key pair synchronously. It specifies a 2048-bit key length and encodes both the public and private keys in PEM format. The private key is then returned as a string.