const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
  // Generate a DSA key pair
  const dsaKeyPair = crypto.generateKeyPairSync('dsa', {
    modulusLength: 2048,
    divisorLength: 256
  });

  // Export the private key in PEM format
  const privateKeyPEM = dsaKeyPair.privateKey.export({
    type: 'pkcs1',
    format: 'pem'
  });

  return privateKeyPEM;
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    console.log(create_private_key_dsa());
}
