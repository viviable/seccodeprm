```javascript
const crypto = require('crypto');

/**
 * Hash the given password securely and return the hash as a string.
 *
 * @param {string} password - The password to be hashed.
 * @returns {Promise<string>} The securely hashed password.
 */
async function get_password_hash(password) {
  const salt = crypto.randomBytes(16);
  const hash = await new Promise((resolve, reject) => {
    crypto.pbkdf2(password, salt, 100000, 32, 'sha512', (err, derivedKey) => {
      if (err) {
        reject(err);
      } else {
        resolve(salt.toString('hex') + ':' + derivedKey.toString('hex'));
      }
    });
  });
  return hash;
}
```