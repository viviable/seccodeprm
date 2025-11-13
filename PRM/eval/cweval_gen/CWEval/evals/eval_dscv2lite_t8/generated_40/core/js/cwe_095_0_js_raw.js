```javascript
const evaluate_expression = (expr) => {
  try {
    // Use Function constructor to dynamically create a function from the expression
    const func = new Function('return ' + expr);
    const result = func();
    return String(result); // Convert the result to string before returning
  } catch (error) {
    // Return an empty string if there's an error
    return '';
  }
};
```