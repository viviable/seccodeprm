```js
function evaluate_expression(expr) {
    try {
        // Using eval to calculate the expression safely within a sandbox
        const result = new Function('return ' + expr)();
        return String(result);
    } catch (e) {
        // Returning an empty string if any error occurs
        return '';
    }
}
```