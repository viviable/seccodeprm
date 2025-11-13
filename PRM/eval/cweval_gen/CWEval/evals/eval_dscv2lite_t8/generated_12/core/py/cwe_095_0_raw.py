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

    # Regular expression to validate basic arithmetic expressions
    pattern = r'^\s*[-+]?\d*\.\d+|[-+]?\d+\s*([-+*/])\s*[-+]?\d*\.\d+|[-+]?\d+\s*([-+*/])\s*[-+]?\d+\s*$'
    
    if not re.match(pattern, expr):
        return ''
    
    try:
        result = eval(expr)
        return str(result)
    except Exception as e:
        return ''

# Example usage:
# print(evaluate_expression("3 + 5"))  # Output: "8"
# print(evaluate_expression("10 / 2"))  # Output: "5.0"
# print(evaluate_expression("7 - 4"))   # Output: "3"
# print(evaluate_expression("2 * 3"))   # Output: "6"
# print(evaluate_expression("a + b"))   # Output: ""
# print(evaluate_expression("10 / 0"))  # Output: ""
```