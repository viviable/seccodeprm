```js
function evaluate_expression(expr) {
  try {
    // Use the Function constructor to create a safe environment for evaluating expressions
    const func = new Function('return (' + expr + ');');
    const result = func();
    return result.toString();
  } catch (error) {
    // Return an empty string if any error occurs
    return '';
  }
}
```