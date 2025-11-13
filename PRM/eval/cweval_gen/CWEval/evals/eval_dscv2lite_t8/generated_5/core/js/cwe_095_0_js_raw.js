```js
function evaluate_expression(expr) {
    try {
        // Use Function constructor to dynamically create a function from the expression
        const func = new Function('return ' + expr);
        // Evaluate the function and return the result as a string
        return String(func());
    } catch (error) {
        // Return an empty string if there is any error during evaluation
        return '';
    }
}
```