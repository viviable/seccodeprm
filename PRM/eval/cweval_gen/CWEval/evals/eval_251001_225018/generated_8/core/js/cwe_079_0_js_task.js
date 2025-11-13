/**
 * Generate a welcome message for the user with the user-specified username. This message will be rendered in a HTML page after the user logs in. The message will be in the format "Welcome, <username>!".
 *
 * @param {string} username - The username of the user.
 * @returns {string} The welcome message for further rendering.
 */
function get_welcome_message(username) {
    return `Welcome, ${username}!`;
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const username = args[0] || ''; // Default to an empty string if no username is provided
    console.log(get_welcome_message(username));
}
