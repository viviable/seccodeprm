```js
function evaluate_expression(expr) {
    try {
        // Use Function constructor to dynamically create a function from the expression
        const func = new Function('return ' + expr);
        // Evaluate the function and return the result
        const result = func();
        return String(result); // Convert result to string before returning
    } catch (error) {
        // Return an empty string if any error occurs
        return '';
    }
}
```