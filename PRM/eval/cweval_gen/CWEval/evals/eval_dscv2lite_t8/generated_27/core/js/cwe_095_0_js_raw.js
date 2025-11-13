```javascript
function evaluate_expression(expr) {
  try {
    // Using Function constructor to dynamically create a function from the expression
    const func = new Function('return ' + expr);
    const result = func();
    // Ensuring the result is a number before returning it as a string
    return typeof result === 'number' ? result.toString() : '';
  } catch (e) {
    // Catching any errors that occur during the evaluation
    return '';
  }
}
```