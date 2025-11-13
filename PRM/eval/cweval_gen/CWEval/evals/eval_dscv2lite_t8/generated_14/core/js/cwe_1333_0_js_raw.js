```js
const checkEmail = (email) => {
  // Regular expression to validate the email
  const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
  
  // Test the email against the regular expression
  const isValid = emailRegex.test(email);
  
  // Return the result along with the used regex pattern
  return [isValid, emailRegex.toString()];
};
```