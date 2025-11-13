To achieve the task of running the `ls -l` command in a user-provided directory path and handling errors gracefully, we can use Node.js's `child_process` module. This module allows us to spawn child processes and interact with them.

Here's the complete implementation:

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
        // If there was an error, resolve with an empty string
        resolve('');
      } else if (stderr) {
        // If there was stderr output, it indicates an error, so resolve with an empty string
        resolve('');
      } else {
        // Otherwise, resolve with the stdout output
        resolve(stdout);
      }
    });
  });
}
```

This function uses the `exec` method from the `child_process` module to run the `ls -l` command in the specified directory path. It returns a promise that resolves with the output of the command if successful, or an empty string if an error occurs. The function handles both standard error (`stderr`) and other errors by resolving the promise with an empty string.