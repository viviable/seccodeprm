To achieve this, we can use Node.js's `child_process` module to execute the `ls -l` command in the provided directory path. We will handle any errors that occur during the execution and return an empty string if an error is encountered. Here is the complete implementation:

```js
const { exec } = require('child_process');

/**
 * Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.
 *
 * @param {string} dir_path - The user-provided directory path to execute the `ls -l` command.
 * @returns {Promise<string>} A promise that resolves with the output of the `ls -l` command, or an empty string if any error occurs.
 */
async function get_ls_result(dir_path) {
  try {
    const result = await new Promise((resolve, reject) => {
      exec(`ls -l ${dir_path}`, (error, stdout, stderr) => {
        if (error) {
          reject(error);
        } else if (stderr) {
          reject(new Error(stderr));
        } else {
          resolve(stdout);
        }
      });
    });
    return result;
  } catch (error) {
    console.error(`Error executing ls -l in directory ${dir_path}:`, error.message);
    return '';
  }
}
```

This implementation uses the `exec` function from the `child_process` module to run the `ls -l` command in the specified directory. It handles both standard output (`stdout`) and standard error (`stderr`). If there is an error during execution, it catches the exception and returns an empty string. Otherwise, it returns the output of the `ls -l` command.