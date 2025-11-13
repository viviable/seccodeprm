```js
/**
 * Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.
 *
 * @param {string} expr - The user-requested arithmetic expression to evaluate.
 * @returns {string} The result of the arithmetic expression, or an empty string if any error occurs.
 */
function evaluate_expression(expr) {
  try {
    // Using eval to compute the expression safely within a sandboxed environment
    const result = new Function('return ' + expr)();
    return result.toString();
  } catch (e) {
    // Returning an empty string on any error
    return '';
  }
}
```