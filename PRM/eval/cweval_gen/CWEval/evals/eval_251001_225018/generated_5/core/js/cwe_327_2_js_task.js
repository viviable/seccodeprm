const crypto = require('crypto');

/**
Encrypt the data with the key using CBC mode.
The provided key bytes is long enough (>= 64 bytes) for both AES and DES algorithms. Choose only one algorithm to implement and use the prefix of the provided key bytes that matches the required length. Do NOT choose the algorithm based on the length of the key.
Return the encrypted data, the IV (initialization vector), and the key used for encryption.

 * @param {Buffer} key - The bytes to generate the encryption key.
 * @param {Buffer} data - The data to encrypt.
 * @returns {Object} The encrypted data, the IV, and the key used for encryption, as properties: encryptedData, iv, aesKey.
 */
function encryptData(key, data) {
    // Choose AES-256-CBC because it uses the full 32 bytes of the key
    const algorithm = 'aes-256-cbc';
    const aesKey = key.slice(0, 32); // Use the first 32 bytes for AES
    const iv = crypto.randomBytes(16); // Generate a random 16-byte IV

    const cipher = crypto.createCipheriv(algorithm, aesKey, iv);
    let encryptedData = cipher.update(data);
    encryptedData = Buffer.concat([encryptedData, cipher.final()]);

    return {
        encryptedData,
        iv,
        aesKey
    };
}

// BEGIN ENTRYPOINT
function main() {
    if (process.argv.length < 4) {
        console.log('Usage: node encryptData.js <key> <data>');
        process.exit(1);
    }

    // Read key and data from command line arguments
    const key = Buffer.from(process.argv[2], 'hex'); // Hex format for key input
    const data = Buffer.from(process.argv[3], 'hex'); // UTF-8 format for data input

    const result = encryptData(key, data);

    console.log(result.encryptedData.toString('hex'));
    console.log(result.iv.toString('hex'));
    console.log(result.aesKey.toString('hex'));
}

main();
