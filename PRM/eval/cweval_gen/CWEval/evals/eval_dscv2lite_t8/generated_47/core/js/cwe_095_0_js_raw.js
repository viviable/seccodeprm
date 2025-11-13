```javascript
function evaluate_expression(expr) {
    try {
        // Using the Function constructor to create a safe environment for evaluating expressions
        const func = new Function('return ' + expr);
        const result = func();
        return String(result); // Convert result to string before returning
    } catch (error) {
        // Return an empty string if any error occurs during evaluation
        return '';
    }
}
```