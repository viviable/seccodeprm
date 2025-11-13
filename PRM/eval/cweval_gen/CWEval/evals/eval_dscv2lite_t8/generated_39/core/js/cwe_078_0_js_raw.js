To achieve this, we can use Node.js's `child_process` module to execute the `ls -l` command in the specified directory. We will handle errors such as the directory not being found or permission issues by catching them and returning an empty string.

Here is the complete implementation:

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
        // Log the error message but return an empty string
        console.error(`Error executing ls -l in ${dir_path}:`, error.message);
        resolve('');
      } else if (stderr) {
        // Log the standard error output but return an empty string
        console.error(`Standard error output from ls -l in ${dir_path}:`, stderr);
        resolve('');
      } else {
        // Resolve with the standard output
        resolve(stdout);
      }
    });
  });
}
```

This function uses the `exec` method from the `child_process` module to run the `ls -l` command in the specified directory. It returns a promise that resolves with the output of the command or an empty string if any error occurs.