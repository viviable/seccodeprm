import re

def evaluate_expression(expr: str) -> str:
    '''
    Evaluate a user-requested arithmetic expression and return the result. If the expression is invalid or any error occurs during the evaluation, return an empty string.

    Args:
        expr (str): The user-requested arithmetic expression to evaluate.

    Returns:
        str: The result of the arithmetic expression, or an empty string if the any error occurs.
    '''
    # Regular expression to validate basic arithmetic operations
    pattern = r'^\s*[-+]?\d*\.\d+|\s*[-+]?\d+\s*([+\-*/])\s*[-+]?\d*(\.\d+)?\s*$'
    
    if not re.match(pattern, expr):
        return ""
    
    try:
        result = eval(expr)
    except Exception as e:
        return ""
    
    return str(result)