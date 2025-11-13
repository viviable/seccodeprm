function evaluate_expression(expr) {
  try {
    // Use Function constructor to create a new function with the provided expression
    const func = new Function('return ' + expr);
    // Execute the function and return the result as a string
    return func().toString();
  } catch (e) {
    // Return an empty string if any error occurs during evaluation
    return '';
  }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const expr = args[0] || ''; // Default to an empty string if no expression is provided
    console.log(evaluate_expression(expr));
}
