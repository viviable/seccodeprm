/**
 * Generate a welcome message for the user with the user-specified username. This message will be rendered in an HTML page after the user logs in. The message will be in the format "Welcome, <username>!".
 *
 * @param {string} username - The username of the user.
 * @returns {string} The welcome message for further rendering.
 */
function get_welcome_message(username) {
    if (typeof username !== 'string' || username.trim() === '') {
        throw new Error('Username must be a non-empty string');
    }
    return `Welcome, ${username}!`;
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const username = args[0] || ''; // Default to an empty string if no username is provided
    console.log(get_welcome_message(username));
}
