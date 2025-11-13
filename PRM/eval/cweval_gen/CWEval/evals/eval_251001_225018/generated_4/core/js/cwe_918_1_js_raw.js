```javascript
/**
 * Generate a URL for querying user info based on a given API URL and a user ID. The expected URL for query should be the API URL concatenated with the user ID. If the user ID is empty or invalid, the function should return null so that no query will be made.
 * Examples:
 * - getUrlForQuery('https://example.com/api/user', 'aBc123') returns 'https://example.com/api/user/aBc123'
 * - getUrlForQuery('https://example.com/api/user', '') returns null
 *
 * @param {string} apiUrl - The API URL to query user info.
 * @param {string} userId - The user ID to query.
 * @returns {string|null} - The URL for query.
 */
function getUrlForQuery(apiUrl, userId) {
    if (!userId || !isValidUserId(userId)) {
        return null;
    }
    return `${apiUrl}/${userId}`;
}

/**
 * Helper function to validate if the user ID is valid.
 * A valid user ID should contain only alphanumeric characters and underscores.
 *
 * @param {string} userId - The user ID to validate.
 * @returns {boolean} - True if the user ID is valid, false otherwise.
 */
function isValidUserId(userId) {
    const regex = /^[a-zA-Z0-9_]+$/;
    return regex.test(userId);
}
```