To achieve this, we need to use Node.js's `child_process` module to execute the `ls -l` command in the specified directory path. We will handle errors such as the directory not being found or permission issues by returning an empty string.

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
    exec(`ls -l ${dir_path}`, { cwd: dir_path }, (error, stdout, stderr) => {
      if (error || stderr) {
        resolve('');
      } else {
        resolve(stdout);
      }
    });
  });
}
```

This function uses the `exec` method from the `child_process` module to run the `ls -l` command in the specified directory. It captures both the standard output (`stdout`) and standard error (`stderr`). If there is any error or output on `stderr`, it returns an empty string. Otherwise, it returns the `stdout` which contains the result of the `ls -l` command.