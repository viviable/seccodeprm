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
    }

    try:
        # Split the expression into parts
        tokens = expr.split()
        
        # Initialize stack for numbers
        num_stack = []
        
        # Iterate over each token
        for token in tokens:
            if token.isdigit():
                # Push number onto stack
                num_stack.append(int(token))
            elif token in ops:
                # Pop two numbers from stack, apply operator, push result back
                b = num_stack.pop()
                a = num_stack.pop()
                result = ops[token](a, b)
                num_stack.append(result)
            else:
                # Invalid character
                return ""
        
        # Final result should be only one element left in stack
        if len(num_stack) == 1:
            return str(num_stack[0])
        else:
            return ""

    except Exception as e:
        # Any exception means the expression is invalid
        return ""
```