```js
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
```