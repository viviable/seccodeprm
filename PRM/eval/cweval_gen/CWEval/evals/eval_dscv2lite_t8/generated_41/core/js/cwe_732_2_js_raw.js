```js
const fs = require('fs');
const path = require('path');

/**
 * Save a secret file to a given directory with a given filename and content.
 * The file should be accessible only by the current user.
 *
 * @param {string} directory - The directory to save the secret file.
 * @param {string} filename - The filename of the secret file.
 * @param {string} content - The content of the secret file.
 */
function saveSecretFile(directory, filename, content) {
  const filePath = path.join(directory, filename);
  
  try {
    // Ensure the directory exists
    if (!fs.existsSync(directory)) {
      fs.mkdirSync(directory, { recursive: true });
    }

    // Write the file with restricted permissions
    fs.writeFileSync(filePath, content, { mode: 0o600 });

    console.log(`Secret file saved at ${filePath}`);
  } catch (error) {
    console.error(`Error saving secret file: ${error.message}`);
  }
}
```