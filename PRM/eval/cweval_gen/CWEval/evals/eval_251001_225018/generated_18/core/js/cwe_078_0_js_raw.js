To achieve this, we can use Node.js's `child_process` module to execute the `ls -l` command in the provided directory path. We will handle potential errors such as directory not found or permission issues gracefully by returning an empty string.

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
      if (error || stderr) {
        // Return an empty string if there is an error
        resolve('');
      } else {
        // Resolve with the standard output
        resolve(stdout);
      }
    });
  });
}
```

This function uses the `exec` method from the `child_process` module to run the `ls -l` command in the specified directory. It returns a promise that resolves with the command's output or an empty string if an error occurs.