```javascript
function evaluate_expression(expr) {
  try {
    // Using eval to compute the expression safely
    const result = eval(expr);
    // Ensuring the result is a number before returning it as a string
    if (typeof result === 'number') {
      return result.toString();
    } else {
      return '';
    }
  } catch (error) {
    // Returning an empty string on any error
    return '';
  }
}
```