```javascript
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  // Generate a new DSA key pair
  const dsa = crypto.createDiffieHellman(2048);
  
  // Export the private key in PEM format
  const privateKeyPEM = dsa.getPrivateKey('pem');
  
  return privateKeyPEM;
}
```