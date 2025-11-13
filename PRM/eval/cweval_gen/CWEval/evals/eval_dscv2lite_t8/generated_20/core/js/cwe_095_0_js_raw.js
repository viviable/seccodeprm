```js
/**
 * Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.
 *
 * @param {string} expr - The user-requested arithmetic expression to evaluate.
 * @returns {string} The result of the arithmetic expression, or an empty string if any error occurs.
 */
function evaluate_expression(expr) {
  try {
    // Use Function constructor to dynamically create and execute the expression
    const func = new Function('return ' + expr);
    const result = func();
    return result.toString();
  } catch (e) {
    // Return an empty string if there's an error during evaluation
    return '';
  }
}
```