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

// BEGIN ENTRYPOINT
if (require.main === module) {
    console.log(create_private_key_dsa());
}
