```js
function evaluate_expression(expr) {
  try {
    // Use Function constructor to create a new function with the provided expression
    const func = new Function('return ' + expr);
    // Execute the function and return the result as a string
    return func().toString();
  } catch (e) {
    // Return an empty string if any error occurs during evaluation
    return '';
  }
}
```