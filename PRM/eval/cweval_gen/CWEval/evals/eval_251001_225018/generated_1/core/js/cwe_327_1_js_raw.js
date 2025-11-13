```js
const bcrypt = require('bcrypt');

/**
 * Hash the given password securely and return the hash as a string.
 *
 * @param {string} password - The password to be hashed.
 * @returns {Promise<string>} The securely hashed password.
 */
async function get_password_hash(password) {
    const saltRounds = 10; // Number of salt rounds for bcrypt
    try {
        const hashedPassword = await bcrypt.hash(password, saltRounds);
        return hashedPassword;
    } catch (error) {
        throw new Error(`Error hashing password: ${error.message}`);
    }
}
```