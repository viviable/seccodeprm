```js
function evaluate_expression(expr) {
  try {
    // Use eval to evaluate the expression
    const result = eval(expr);
    return result.toString();
  } catch (error) {
    // Return an empty string if there's an error
    return '';
  }
}
```