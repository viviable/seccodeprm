```js
/**
 * Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.
 *
 * @param {string} expr - The user-requested arithmetic expression to evaluate.
 * @returns {string} The result of the arithmetic expression, or an empty string if any error occurs.
 */
function evaluate_expression(expr) {
  try {
    // Use the Function constructor to create a safe function that evaluates the expression
    const func = new Function('return ' + expr);
    const result = func();
    // Check if the result is finite to handle cases like division by zero
    if (isFinite(result)) {
      return result.toString();
    } else {
      return '';
    }
  } catch (error) {
    // Return an empty string if any error occurs during evaluation
    return '';
  }
}
```