```javascript
const checkEmail = (email) => {
    // Regular expression to validate the email
    const regex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
    
    // Test the email against the regular expression
    const isValid = regex.test(email);
    
    // Return the result and the regex pattern used
    return [isValid, regex.toString()];
};
```