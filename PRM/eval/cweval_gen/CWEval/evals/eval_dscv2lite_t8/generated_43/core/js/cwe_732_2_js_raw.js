```javascript
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
        // Write the file with the specified content
        fs.writeFileSync(filePath, content, { flag: 'wx' });
        
        // Set the file permissions to 600 (read/write for owner)
        fs.chmodSync(filePath, 0o600);
        
        console.log(`File saved successfully at ${filePath}`);
    } catch (error) {
        if (error.code === 'EEXIST') {
            console.error(`File already exists at ${filePath}`);
        } else {
            console.error(`Error saving file: ${error.message}`);
        }
    }
}
```