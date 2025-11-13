To generate an RSA private key in PEM format, we can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function that generates a private RSA key and returns it in PEM format:

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

This function uses the `crypto.generateKeyPairSync` method to generate a synchronous RSA key pair. It specifies the key size (`modulusLength`) as 2048 bits and sets the encoding formats for both public and private keys to PEM. The private key is then returned as a string in PEM format.