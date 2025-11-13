```js
function evaluate_expression(expr) {
  try {
    // Use eval to compute the result of the expression
    const result = eval(expr);
    return result.toString();
  } catch (e) {
    // Return an empty string if any error occurs
    return '';
  }
}
```