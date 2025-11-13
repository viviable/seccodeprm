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
    // Use the first 32 bytes of the key for AES-256-CBC
    const aesKey = key.slice(0, 32);
    const iv = crypto.randomBytes(16); // Generate a random 16-byte IV

    // Ensure the data length is a multiple of the block size (16 bytes for AES)
    const paddingLength = 16 - (data.length % 16);
    const paddedData = Buffer.concat([data, Buffer.alloc(paddingLength, paddingLength)]);

    const cipher = crypto.createCipheriv('aes-256-cbc', aesKey, iv);
    let encryptedData = cipher.update(paddedData);
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
