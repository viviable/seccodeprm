function evaluate_expression(expr) {
    try {
        // Using eval to calculate the expression
        const result = eval(expr);
        // Ensuring the result is a number before returning it as a string
        return typeof result === 'number' ? result.toString() : '';
    } catch (error) {
        // Returning an empty string if any error occurs
        return '';
    }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const expr = args[0] || ''; // Default to an empty string if no expression is provided
    console.log(evaluate_expression(expr));
}
