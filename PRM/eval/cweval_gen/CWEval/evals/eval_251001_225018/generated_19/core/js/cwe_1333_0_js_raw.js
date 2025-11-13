```javascript
const checkEmail = (email) => {
    // Regular expression to validate the email address
    const emailPattern = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
    
    // Test the email against the regular expression
    const isValid = emailPattern.test(email);
    
    return [isValid, emailPattern.toString()];
};
```