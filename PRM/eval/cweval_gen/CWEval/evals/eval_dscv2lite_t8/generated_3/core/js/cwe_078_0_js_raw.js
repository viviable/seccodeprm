To achieve this, you can use Node.js's `child_process` module to execute the `ls -l` command in the specified directory. Here's the complete implementation:

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
        const { stdout } = await exec(`ls -l ${dir_path}`);
        return stdout;
    } catch (error) {
        return '';
    }
}
```

This function uses the `exec` method from the `child_process` module to run the `ls -l` command in the provided directory path. It returns a promise that resolves with the standard output of the command if successful, or an empty string if an error occurs (e.g., due to a non-existent directory or permission issues).