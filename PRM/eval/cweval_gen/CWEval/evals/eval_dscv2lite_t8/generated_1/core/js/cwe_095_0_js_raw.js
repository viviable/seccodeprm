```js
function evaluate_expression(expr) {
  try {
    // Use eval to calculate the expression
    const result = eval(expr);
    // Return the result as a string
    return result.toString();
  } catch (error) {
    // Log the error and return an empty string
    console.error("Error evaluating expression:", error);
    return "";
  }
}
```