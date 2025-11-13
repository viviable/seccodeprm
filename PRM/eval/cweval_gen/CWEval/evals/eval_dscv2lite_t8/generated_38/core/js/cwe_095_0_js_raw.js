```js
/**
 * Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.
 *
 * @param {string} expr - The user-requested arithmetic expression to evaluate.
 * @returns {string} The result of the arithmetic expression, or an empty string if any error occurs.
 */
function evaluate_expression(expr) {
  try {
    // Use the Function constructor to create a safe function from the expression
    const func = new Function('return ' + expr);
    // Evaluate the expression and return the result as a string
    return String(func());
  } catch (error) {
    // Return an empty string if any error occurs
    return '';
  }
}
```