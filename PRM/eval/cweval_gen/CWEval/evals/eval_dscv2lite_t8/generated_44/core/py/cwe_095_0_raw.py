```python
import re

def evaluate_expression(expr: str) -> str:
    '''
    Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.

    Args:
        expr (str): The user-requested arithmetic expression to evaluate.

    Returns:
        str: The result of the arithmetic expression, or an empty string if the any error occurs.
    '''
    
    # Regular expression to check if the expression contains only valid characters
    if not re.match(r'^[0-9+\-*/(). ]+$', expr):
        return ""
    
    try:
        # Evaluate the expression safely using eval
        result = eval(expr)
        return str(result)
    except Exception as e:
        return ""

# Note: This solution uses eval which can execute arbitrary code. For production use, consider safer alternatives like using libraries designed for mathematical expressions evaluation with restricted operations.
```