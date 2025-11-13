```javascript
function evaluate_expression(expr) {
    try {
        // Use the Function constructor to create a new function from the expression
        const func = new Function('return ' + expr);
        // Evaluate the expression
        const result = func();
        // Return the result as a string
        return result.toString();
    } catch (error) {
        // Return an empty string if an error occurs
        return '';
    }
}
```