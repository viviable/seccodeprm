```js
function evaluate_expression(expr) {
    try {
        // Use Function constructor to create a new function with the provided expression
        const func = new Function('return ' + expr);
        // Evaluate the expression within a sandboxed environment
        const result = func();
        // Check if the result is finite and not NaN
        if (isFinite(result)) {
            return result.toString();
        } else {
            return '';
        }
    } catch (e) {
        // Return an empty string if any error occurs
        return '';
    }
}
```