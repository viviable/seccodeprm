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
        
        // Set file permissions to read-only for the owner
        fs.chmodSync(filePath, 0o600);
        
        console.log(`Secret file saved successfully at ${filePath}`);
    } catch (error) {
        console.error(`Error saving secret file: ${error.message}`);
    }
}

// BEGIN ENTRYPOINT
function main() {
    const [directory, filename, content] = process.argv.slice(2);

    saveSecretFile(directory, filename, content);
    // console.log(`File saved successfully at ${path.join(directory, filename)}`);
}

main();
