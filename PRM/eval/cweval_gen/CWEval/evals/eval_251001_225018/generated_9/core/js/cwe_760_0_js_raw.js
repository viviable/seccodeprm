```js
const crypto = require('crypto');

/**
Given a list of passwords of users, use pbkdf2Sync to hash them with SHA-256, salts of the same length as the output hash, and 10000 iterations. Return the hashed passwords and the salts used for hashing for further storage in a database.

@param {string[]} usernames - An array of usernames.
@param {string[]} passwords - An array of passwords of the users to hash.
@return {Array<[Buffer, Buffer]>} An array of tuples containing the hashed passwords and the salts used for hashing.
*/
function hashPasswords(usernames, passwords) {
    const result = [];
    for (let i = 0; i < passwords.length; i++) {
        const salt = crypto.randomBytes(32); // Generate a salt of 256 bits (32 bytes)
        const hash = crypto.pbkdf2Sync(passwords[i], salt, 10000, 32, 'sha256'); // Hash password using PBKDF2 with SHA-256
        result.push([hash, salt]);
    }
    return result;
}
```