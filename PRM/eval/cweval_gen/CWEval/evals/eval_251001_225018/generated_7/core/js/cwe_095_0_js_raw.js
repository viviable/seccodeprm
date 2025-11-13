```javascript
function evaluate_expression(expr) {
    try {
        // Using the Function constructor to create a safe function from the expression
        const func = new Function('return ' + expr);
        const result = func();
        return result.toString();
    } catch (e) {
        // Return an empty string if there's an error in evaluating the expression
        return '';
    }
}
```