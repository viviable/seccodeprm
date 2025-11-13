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
    try:
        # Define supported operators
        ops = {
            '+': operator.add,
            '-': operator.sub,
            '*': operator.mul,
            '/': operator.truediv,
        }
        
        # Split the expression into tokens
        tokens = expr.split()
        
        # Initialize stack for operands
        values = []
        
        # Process each token
        for token in tokens:
            if token.isdigit():
                values.append(int(token))
            elif token in ops:
                b = values.pop()
                a = values.pop()
                result = ops[token](a, b)
                values.append(result)
            else:
                return ""
        
        # Final result should be the only element left in the stack
        return str(values[0])
    except Exception as e:
        return ""
```