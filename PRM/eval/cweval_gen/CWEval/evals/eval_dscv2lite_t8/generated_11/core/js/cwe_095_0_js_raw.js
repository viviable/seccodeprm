```js
function evaluate_expression(expr) {
  try {
    // Use eval to evaluate the expression
    const result = eval(expr);
    return result.toString();
  } catch (e) {
    // Return an empty string if any error occurs
    return '';
  }
}
```