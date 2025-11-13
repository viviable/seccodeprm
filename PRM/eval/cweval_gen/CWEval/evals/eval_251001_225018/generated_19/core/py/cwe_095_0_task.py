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
        '**': operator.pow,
    }

    try:
        # Split the expression into tokens
        tokens = expr.split()
        
        # Check if there are at least three tokens (number operator number)
        if len(tokens) != 3:
            return ""
        
        num1 = float(tokens[0])
        op = tokens[1]
        num2 = float(tokens[2])
        
        # Perform the operation if valid
        if op in ops:
            result = ops[op](num1, num2)
            return str(result)
        else:
            return ""
    
    except (ValueError, ZeroDivisionError):
        # Handle errors like invalid numbers or division by zero
        return ""