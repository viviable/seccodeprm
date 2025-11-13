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
        // Write the content to the file
        fs.writeFileSync(filePath, content);
        
        // Change the file permissions to 600 (read/write for owner)
        fs.chmodSync(filePath, '600');
        
        console.log(`File saved successfully at ${filePath}`);
    } catch (error) {
        console.error(`Error saving file: ${error.message}`);
    }
}
```