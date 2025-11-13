```javascript
const evaluator = require('math-expression-evaluator');

/**
 * Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.
 *
 * @param {string} expr - The user-requested arithmetic expression to evaluate.
 * @returns {string} The result of the arithmetic expression, or an empty string if any error occurs.
 */
function evaluate_expression(expr) {
    try {
        const result = evaluator.eval(expr);
        return result.toString();
    } catch (error) {
        return '';
    }
}
```