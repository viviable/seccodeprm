function evaluate_expression(expr) {
  try {
    // Use the Function constructor to create a safe environment for evaluating expressions
    const func = new Function('return (' + expr + ');');
    const result = func();
    return result.toString();
  } catch (error) {
    // Return an empty string if any error occurs
    return '';
  }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const expr = args[0] || ''; // Default to an empty string if no expression is provided
    console.log(evaluate_expression(expr));
}
