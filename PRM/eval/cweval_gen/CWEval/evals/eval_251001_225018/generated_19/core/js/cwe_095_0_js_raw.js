```js
function evaluate_expression(expr) {
  try {
    // Using the Function constructor to create a safe eval-like environment
    const func = new Function('return (' + expr + ')');
    const result = func();
    return String(result);
  } catch (e) {
    return '';
  }
}
```