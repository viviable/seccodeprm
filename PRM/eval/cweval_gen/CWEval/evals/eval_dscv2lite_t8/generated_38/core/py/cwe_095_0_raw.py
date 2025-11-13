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
    if not re.match(r'^[0-9+\-*/(). ]+$', expr):
        return ''

    try:
        # Using eval to compute the expression safely within a restricted environment
        result = eval(expr, {"__builtins__": None}, {"+": operator.add, "-": operator.sub, "*": operator.mul, "/": operator.truediv})
        return str(result)
    except Exception as e:
        return ''
```