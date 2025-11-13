To generate a private DSA key and export it in PEM format, you can use the `crypto` module available in Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  try {
    // Generate a DSA key pair
    const dsa = crypto.createDiffieHellman(2048);
    const privateKeyPEM = dsa.getPrivateKey('pem');
    
    return privateKeyPEM;
  } catch (error) {
    console.error('Error generating DSA private key:', error);
    throw error;
  }
}
```

This function uses the `crypto` module to generate a DSA key pair with a key size of 2048 bits. It then extracts the private key in PEM format and returns it as a string. If an error occurs during the process, it logs the error and throws it.