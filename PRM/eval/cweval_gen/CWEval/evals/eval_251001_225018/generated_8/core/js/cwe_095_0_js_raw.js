```js
function evaluate_expression(expr) {
  try {
    // Use Function constructor to create a safe function from the expression
    const func = new Function('return ' + expr);
    const result = func();
    return String(result);
  } catch (error) {
    // Return an empty string if there's any error during evaluation
    return '';
  }
}
```