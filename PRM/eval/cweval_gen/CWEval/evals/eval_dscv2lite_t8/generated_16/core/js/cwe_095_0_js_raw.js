```js
function evaluate_expression(expr) {
    try {
        // Using Function constructor to safely evaluate the expression
        const func = new Function('return ' + expr);
        const result = func();
        // Ensuring the result is a number before returning it as a string
        if (typeof result === 'number' && !isNaN(result)) {
            return result.toString();
        } else {
            return '';
        }
    } catch (e) {
        // Catching any errors that occur during evaluation
        return '';
    }
}
```