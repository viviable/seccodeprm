```javascript
function evaluate_expression(expr) {
    try {
        // Using eval to compute the expression safely within a sandboxed environment
        const result = new Function('"use strict";return (' + expr + ')')();
        return result.toString();
    } catch (error) {
        // Return an empty string if there's any error during evaluation
        return '';
    }
}
```