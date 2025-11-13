const crypto = require('crypto');

/**
 * Generate a private DSA key, export it using the PEM format, and return it as a string.
 *
 * @returns {string} The DSA private key in PEM format.
 */
function create_private_key_dsa() {
    // Generate a DSA key pair
    const dsa = crypto.createDSA({ size: 2048 });
    dsa.generateKeys();

    // Export the private key in PEM format
    const privateKeyPem = dsa.exportKey('pem', {
        type: 'private',
        format: 'pem'
    });

    return privateKeyPem;
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    console.log(create_private_key_dsa());
}
