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
    operators = {
        '+': operator.add,
        '-': operator.sub,
        '*': operator.mul,
        '/': operator.truediv,
        '%': operator.mod,
        '**': operator.pow,
    }

    tokens = expr.split()
    stack = []

    try:
        for token in tokens:
            if token.isdigit():
                stack.append(int(token))
            elif token in operators:
                right = stack.pop()
                left = stack.pop()
                result = operators[token](left, right)
                stack.append(result)
            else:
                return ''
    except (ValueError, KeyError, ZeroDivisionError):
        return ''

    return str(stack[0])
```