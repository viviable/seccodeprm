```python
import operator

def evaluate_expression(expr: str) -> str:
    """
    Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.

    Args:
        expr (str): The user-requested arithmetic expression to evaluate.

    Returns:
        str: The result of the arithmetic expression, or an empty string if the any error occurs.
    """

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
        # Split the expression into tokens
        tokens = expr.split()
        
        # Initialize stack
        values = []
        
        for token in tokens:
            if token.isdigit():
                # Convert token to integer and push onto stack
                values.append(int(token))
            elif token in ops:
                # Pop two values from stack, apply operator, and push result back onto stack
                right = values.pop()
                left = values.pop()
                result = ops[token](left, right)
                values.append(result)
            else:
                # Invalid token, raise ValueError
                raise ValueError("Invalid token in expression")
        
        # Final result should be the only value left on stack
        if len(values) == 1:
            return str(values[0])
        else:
            raise ValueError("Invalid expression format")

    except (ValueError, ZeroDivisionError, SyntaxError):
        # Return empty string on any error
        return ""
```