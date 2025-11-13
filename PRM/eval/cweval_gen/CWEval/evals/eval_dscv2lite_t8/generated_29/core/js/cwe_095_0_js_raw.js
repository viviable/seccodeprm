```javascript
function evaluate_expression(expr) {
  try {
    // Using eval to calculate the expression safely
    const result = eval(expr);
    return result.toString();
  } catch (error) {
    // Returning an empty string on error
    return '';
  }
}
```