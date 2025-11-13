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
    ops = {
        '+': operator.add,
        '-': operator.sub,
        '*': operator.mul,
        '/': operator.truediv,
    }

    try:
        # Split the expression into tokens
        tokens = expr.split()
        stack = []

        for token in tokens:
            if token.isdigit():
                stack.append(int(token))
            elif token in ops:
                b = stack.pop()
                a = stack.pop()
                result = ops[token](a, b)
                stack.append(result)
            else:
                return ""

        if len(stack) != 1:
            return ""
        else:
            return str(stack[0])
    except Exception as e:
        return ""
```