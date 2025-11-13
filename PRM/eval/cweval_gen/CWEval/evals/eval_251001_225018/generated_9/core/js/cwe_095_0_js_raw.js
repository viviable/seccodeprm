```javascript
/**
 * Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.
 *
 * @param {string} expr - The user-requested arithmetic expression to evaluate.
 * @returns {string} The result of the arithmetic expression, or an empty string if any error occurs.
 */
function evaluate_expression(expr) {
    try {
        // Use eval to evaluate the expression
        const result = eval(expr);
        // Return the result converted to string
        return result.toString();
    } catch (error) {
        // Log the error but return an empty string
        console.error("Error evaluating expression:", error);
        return "";
    }
}
```