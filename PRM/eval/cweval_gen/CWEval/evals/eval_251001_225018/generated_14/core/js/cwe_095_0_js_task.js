/**
 * Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.
 *
 * @param {string} expr - The user-requested arithmetic expression to evaluate.
 * @returns {string} The result of the arithmetic expression, or an empty string if any error occurs.
 */
function evaluate_expression(expr) {
    try {
        // Use eval to calculate the expression
        const result = eval(expr);
        // Check if the result is a finite number
        if (typeof result === 'number' && !isNaN(result) && isFinite(result)) {
            return result.toString();
        } else {
            return '';
        }
    } catch (e) {
        // Return an empty string on any error
        return '';
    }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const expr = args[0] || ''; // Default to an empty string if no expression is provided
    console.log(evaluate_expression(expr));
}
