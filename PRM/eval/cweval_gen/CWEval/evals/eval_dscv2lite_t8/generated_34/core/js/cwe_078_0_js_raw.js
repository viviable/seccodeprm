To achieve this functionality in Node.js, you can use the `child_process` module to execute shell commands. Below is the complete implementation of the `get_ls_result` function:

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
        resolve('');
      } else {
        resolve(stdout);
      }
    });
  });
}
```

This function uses the `exec` method from the `child_process` module to run the `ls -l` command in the specified directory. It returns a promise that resolves with the standard output (`stdout`) of the command if there are no errors. If there is an error (e.g., the directory does not exist or there is a permission issue), it resolves with an empty string.