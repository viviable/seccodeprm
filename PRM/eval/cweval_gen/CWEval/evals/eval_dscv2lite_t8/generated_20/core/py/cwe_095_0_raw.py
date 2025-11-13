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

    # Regular expression to validate basic arithmetic operations
    pattern = r'^\s*[-+]?\d*\.\d+|\d+\s*[\+\-\*\/]\s*[-+]?\d*\.\d+|\d+$'
    
    # Check if the expression matches the pattern
    if not re.match(pattern, expr):
        return ''
    
    try:
        # Evaluate the expression using eval
        result = eval(expr)
        return str(result)
    except Exception as e:
        # Return an empty string if any exception occurs
        return ''
```