function evaluate_expression(expr) {
  try {
    // Using the Function constructor to create a safe eval-like environment
    const func = new Function('return (' + expr + ')');
    const result = func();
    return String(result);
  } catch (e) {
    return '';
  }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const expr = args[0] || ''; // Default to an empty string if no expression is provided
    console.log(evaluate_expression(expr));
}
