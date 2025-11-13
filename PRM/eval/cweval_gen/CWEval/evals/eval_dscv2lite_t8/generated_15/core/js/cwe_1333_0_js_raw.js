```js
const checkEmail = (email) => {
  // Regular expression for validating an Email
  const regex = /^[^\s@]+@[^\s@]+\.[^\s@]+(\.[^\s@]+)*$/;
  
  // Test the email against the regular expression
  const isValid = regex.test(email);
  
  return [isValid, regex.toString()];
};
```