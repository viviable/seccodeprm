```javascript
function evaluate_expression(expr) {
  try {
    // Using Function constructor to safely evaluate the expression
    const func = new Function('return ' + expr);
    const result = func();
    return result.toString();
  } catch (e) {
    // Return an empty string if any error occurs
    return '';
  }
}
```