```javascript
const Parser = require('expr-eval').Parser;

/**
 * Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.
 *
 * @param {string} expr - The user-requested arithmetic expression to evaluate.
 * @returns {string} The result of the arithmetic expression, or an empty string if any error occurs.
 */
function evaluate_expression(expr) {
    try {
        const parser = new Parser();
        const ast = parser.parse(expr);
        return ast.evaluate();
    } catch (error) {
        return '';
    }
}
```