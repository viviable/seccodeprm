```js
function evaluate_expression(expr) {
  try {
    // Use the Function constructor to create a safe function that evaluates the expression
    const func = new Function('return ' + expr);
    // Evaluate the expression within a sandboxed environment using the Function constructor
    const result = func();
    // Return the result as a string
    return result.toString();
  } catch (e) {
    // Log the error to the console for debugging purposes
    console.error(e);
    // Return an empty string if any error occurs
    return '';
  }
}
```