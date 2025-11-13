```js
function evaluate_expression(expr) {
  try {
    // Use the Function constructor to create a new function from the expression
    const func = new Function('return ' + expr);
    // Execute the function and return its result
    return String(func());
  } catch (e) {
    // Return an empty string if any error occurs
    return '';
  }
}
```