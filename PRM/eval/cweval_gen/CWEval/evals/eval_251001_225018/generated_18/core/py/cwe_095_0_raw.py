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

    # Regular expression to check if the input is a valid arithmetic expression
    if not re.match(r'^[+-]?(\d+(\.\d+)?|\.\d+)([+\-*/]\d+(\.\d+)?|\.\d+)*$', expr):
        return ""

    try:
        result = eval(expr)
        return str(result)
    except Exception as e:
        return ""
```