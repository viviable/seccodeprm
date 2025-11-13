```js
function evaluate_expression(expr) {
  try {
    // Using the Function constructor to create a safe function from the expression
    const func = new Function('return ' + expr);
    const result = func();
    return isNaN(result) ? '' : result.toString();
  } catch (e) {
    // Return an empty string if any error occurs during evaluation
    return '';
  }
}
```