To generate a private DSA key and export it using the PEM format in JavaScript, you can use the `crypto` module provided by Node.js. Below is the complete implementation of the `create_private_key_dsa` function:

```js
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  // Generate a new DSA key pair
  const dsa = crypto.createDiffieHellman(512); // Using 512 bits for demonstration purposes

  // Export the private key in PEM format
  const privateKeyPem = dsa.getPrivateKey('pem');

  return privateKeyPem;
}
```

This function uses the `crypto` module to generate a new DSA key pair with a 512-bit prime modulus. It then exports the private key in PEM format and returns it as a string. Note that the bit length (512 in this case) can be adjusted based on security requirements.