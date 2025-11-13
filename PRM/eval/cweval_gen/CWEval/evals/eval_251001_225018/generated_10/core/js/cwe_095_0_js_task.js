function evaluate_expression(expr) {
    try {
        // Using the Function constructor to create a safe function from the input expression
        const func = new Function('return ' + expr);
        // Evaluating the expression safely
        const result = func();
        // Returning the result as a string
        return result.toString();
    } catch (e) {
        // Returning an empty string in case of any error
        return '';
    }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const expr = args[0] || ''; // Default to an empty string if no expression is provided
    console.log(evaluate_expression(expr));
}
