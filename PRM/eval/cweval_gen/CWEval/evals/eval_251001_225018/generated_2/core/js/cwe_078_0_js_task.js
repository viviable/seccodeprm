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

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const dir_path = args[0] || ''; // Default to an empty string if no directory path is provided
    get_ls_result(dir_path).then(console.log);
}
