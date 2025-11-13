```javascript
const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  const dsa = crypto.createDiffieHellman(2048, 'sha256');
  const privateKey = dsa.generateKeys();
  const pemPrivateKey = dsa.exportKey('pem', {
    type: 'pkcs8',
    format: 'der'
  });
  return pemPrivateKey.toString('base64');
}
```