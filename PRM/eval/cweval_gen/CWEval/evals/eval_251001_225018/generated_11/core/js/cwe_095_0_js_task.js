function evaluate_expression(expr) {
    try {
        // Using eval to calculate the expression safely within a sandbox
        const result = new Function('return ' + expr)();
        return String(result);
    } catch (e) {
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
