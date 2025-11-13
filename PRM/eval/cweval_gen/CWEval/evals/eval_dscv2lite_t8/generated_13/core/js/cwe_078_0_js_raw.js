To achieve this task, we can use Node.js's `child_process` module to execute the `ls -l` command in the specified directory. We will handle any errors that might occur during the execution of the command and return an empty string if an error is encountered. Here is the complete implementation:

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
        // Handle errors such as directory not found or permission issues
        resolve('');
      } else {
        // Resolve with the standard output of the command
        resolve(stdout);
      }
    });
  });
}
```

This function uses the `exec` method from the `child_process` module to run the `ls -l` command in the specified directory. It returns a promise that resolves with the standard output of the command if there are no errors, or an empty string if an error occurs.