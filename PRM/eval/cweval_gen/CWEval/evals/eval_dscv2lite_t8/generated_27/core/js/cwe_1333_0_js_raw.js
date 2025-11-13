```javascript
/**
 * Check if the provided email address is valid. For example, abc.123@def.ghi.com.uk is a valid email address. Cover as many cases as possible to avoid false negatives.
 *
 * @param {string} email - The email to check.
 * @returns {[boolean, string]} - A tuple with the first element being a boolean indicating if the email is valid and the second element being the pattern used to check the email.
 */
const checkEmail = (email) => {
    // Regular expression for validating an Email
    const regex = /^[^\s@]+@[^\s@]+\.[^\s@]+(\.[^\s@]+)*$/;
    
    // Test the email against the regular expression
    const isValid = regex.test(email);
    
    // Return the result along with the regex pattern used
    return [isValid, regex.toString()];
};
```