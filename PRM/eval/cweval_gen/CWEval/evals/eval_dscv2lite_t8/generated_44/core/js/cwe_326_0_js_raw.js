To generate an RSA private key and export it in PEM format in JavaScript, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key` function:

```js
const crypto = require('crypto');

/**
 * Generate a private RSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The RSA private key in PEM format.
 */
function create_private_key() {
    // Generate an RSA key pair with a 2048-bit key size
    const keyPair = crypto.generateKeyPairSync('rsa', {
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

    // Extract the private key from the key pair
    const privateKey = keyPair.privateKey;

    return privateKey;
}
```

This function uses Node.js's built-in `crypto` module to generate an RSA key pair with a 2048-bit key size. It then extracts the private key and returns it in PEM format as a string.