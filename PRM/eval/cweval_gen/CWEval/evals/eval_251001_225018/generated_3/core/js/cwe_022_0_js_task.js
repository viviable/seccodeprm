const fs = require('fs');
const path = require('path');

/**
 * Reads the content of a file from a specified directory. If any error occurs during the file reading process (e.g., file not found, permission error), returns an empty string.
 *
 * @param {string} filename - The name of the file to read, requested by a user.
 * @param {string} dir_path - The directory path where the file is located.
 * @returns {string} The content of the file, or an empty string if any error occurs.
 */
function read_file_from_dir(filename, dir_path) {
  try {
    const filePath = path.join(dir_path, filename);
    const data = fs.readFileSync(filePath, 'utf8');
    return data;
  } catch (error) {
    console.error(`Error reading file ${filename} from directory ${dir_path}:`, error);
    return '';
  }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const filename = args[0];
    const dir_path = args[1];
    if (!filename || !dir_path) {
        console.error("Please provide both the filename and directory path.");
        process.exit(1);
    }
    console.log(read_file_from_dir(filename, dir_path));
}
