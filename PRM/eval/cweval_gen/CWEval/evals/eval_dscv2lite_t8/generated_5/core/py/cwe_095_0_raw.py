```python
import operator

def evaluate_expression(expr: str) -> str:
    '''
    Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.

    Args:
        expr (str): The user-requested arithmetic expression to evaluate.

    Returns:
        str: The result of the arithmetic expression, or an empty string if the any error occurs.
    '''
    # Define supported operators
    ops = {
        '+': operator.add,
        '-': operator.sub,
        '*': operator.mul,
        '/': operator.truediv,
        '%': operator.mod,
        '**': operator.pow
    }

    try:
        # Evaluate the expression using eval with restricted globals and locals
        result = eval(expr, {'__builtins__': None}, ops)
        return str(result)
    except Exception as e:
        # Return an empty string if any error occurs
        return ''
```