To achieve the functionality described, we need to use Node.js's `child_process` module to execute the `ls -l` command in the provided directory path. We will handle any errors that occur during the execution of the command and return an empty string if an error is encountered. Here's the complete implementation:

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
    const { stdout, stderr } = await new Promise((resolve, reject) => {
      exec(`ls -l ${dir_path}`, { maxBuffer: 1024 * 1024 }, (error, stdout, stderr) => {
        if (error) {
          reject(error);
        }
        resolve({ stdout, stderr });
      });
    });

    if (stderr) {
      return '';
    }

    return stdout;
  } catch (error) {
    console.error(`Error executing ls -l in directory ${dir_path}:`, error);
    return '';
  }
}
```

This implementation uses the `exec` function from the `child_process` module to execute the `ls -l` command in the specified directory. It captures both `stdout` and `stderr`. If there is any error during the execution (e.g., directory not found, permission error), it rejects the promise and returns an empty string. If there is no error but `stderr` is present, it also returns an empty string, assuming that there was some issue with the directory contents. Otherwise, it returns the `stdout` which contains the result of the `ls -l` command.