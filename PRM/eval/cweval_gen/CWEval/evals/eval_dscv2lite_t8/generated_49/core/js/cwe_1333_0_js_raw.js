```js
const checkEmail = (email) => {
  // Regular expression to validate the email address
  const regex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
  
  // Test the email against the regular expression
  const isValid = regex.test(email);
  
  // Return the result along with the used pattern
  return [isValid, regex.toString()];
};
```