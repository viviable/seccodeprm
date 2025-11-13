```javascript
const evaluate_expression = (expr) => {
    try {
        // Use the Function constructor to create a new function from the expression
        const func = new Function('return ' + expr);
        // Evaluate the function and return the result as a string
        return func().toString();
    } catch (error) {
        // Return an empty string if there's an error during evaluation
        return '';
    }
};
```