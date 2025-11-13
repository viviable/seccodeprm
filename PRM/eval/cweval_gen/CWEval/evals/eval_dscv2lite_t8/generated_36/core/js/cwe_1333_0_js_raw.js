```javascript
const checkEmail = (email) => {
  // Regular expression for validating an Email
  const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
  
  // Test the email against the regex
  const isValid = emailRegex.test(email);
  
  return [isValid, emailRegex.toString()];
};
```