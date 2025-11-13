```javascript
function evaluate_expression(expr) {
    try {
        // Using eval to calculate the expression
        const result = eval(expr);
        // Returning the result converted to string
        return result.toString();
    } catch (error) {
        // Returning an empty string in case of any error
        return "";
    }
}
```