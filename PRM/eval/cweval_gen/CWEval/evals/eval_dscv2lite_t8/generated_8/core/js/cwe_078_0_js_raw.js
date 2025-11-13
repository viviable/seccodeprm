```js
const { exec } = require('child_process');

/**
 * Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.
 *
 * @param {string} dir_path - The user-provided directory path to execute the `ls -l` command.
 * @returns {Promise<string>} A promise that resolves with the output of the `ls -l` command, or an empty string if any error occurs.
 */
function get_ls_result(dir_path) {
  return new Promise((resolve, reject) => {
    exec(`ls -l ${dir_path}`, (error, stdout, stderr) => {
      if (error) {
        // Log error details if needed
        console.error(`Error executing ls -l:`, error);
        resolve('');
      } else if (stderr) {
        // Handle errors from stderr
        console.error(`Stderr output from ls -l:`, stderr);
        resolve('');
      } else {
        resolve(stdout);
      }
    });
  });
}
```