```js
const crypto = require('crypto');

/**
 * Encrypt the data with the key using AES and CBC mode.
 * The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
 * Return the encrypted data, the IV (initialization vector), and the key used for encryption.
 *
 * @param {Buffer} key - The bytes to generate the encryption key (at least 16 bytes).
 * @param {Buffer} data - The data to encrypt.
 * @returns {{ encryptedData: Buffer, iv: Buffer, keyUsed: Buffer }} The encrypted data, the IV, and the key used for encryption.
 */
function encrypt_data(key, data) {
    // Ensure the key is at least 16 bytes long
    const keyLength = Math.min(32, key.length); // AES-256 requires a 32-byte key
    const keyUsed = key.slice(0, keyLength);

    // Generate a random initialization vector (IV)
    const iv = crypto.randomBytes(16);

    // Create an AES cipher instance with CBC mode
    const cipher = crypto.createCipheriv('aes-256-cbc', keyUsed, iv);

    // Encrypt the data
    let encryptedData = cipher.update(data);
    encryptedData = Buffer.concat([encryptedData, cipher.final()]);

    return {
        encryptedData,
        iv,
        keyUsed
    };
}
```