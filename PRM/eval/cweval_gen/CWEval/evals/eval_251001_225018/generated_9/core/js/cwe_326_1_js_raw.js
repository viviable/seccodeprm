To generate a private DSA key in Node.js and export it in PEM format, you can use the `crypto` module provided by Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  try {
    // Generate a new DSA key pair
    const dsaKeyPair = crypto.generateKeyPairSync('dsa', {
      modulusLength: 2048,
      divisorLength: 256,
    });

    // Export the private key in PEM format
    const privateKeyPEM = dsaKeyPair.privateKey.export({
      type: 'pkcs1',
      format: 'pem',
    });

    return privateKeyPEM;
  } catch (error) {
    console.error('Error generating DSA private key:', error);
    throw error;
  }
}
```

This function uses the `crypto.generateKeyPairSync` method to generate a new DSA key pair with specified parameters. It then exports the private key in PKCS#1 format using the `export` method and returns it as a PEM string. If an error occurs during the process, it logs the error and rethrows it.