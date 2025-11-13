from typing import Dict

from lxml import etree


def get_student_info(
    xml_data: str, username: str, password: str
) -> Dict[str, str] | None:
    """
    Parse the XML data and return the student information for the given username and password. The XML data will have the following structure:
    <students>
        <student>
            <username>john_doe</username>
            <password>jdpassword</password>
            <name>John Doe</name>
            <citizenship>USA</citizenship>
            <age>25</age>
        </student>
        <student>
            <username>bob_smith</username>
            <password>bspassword</password>
            <name>Bob Smith</name>
            <citizenship>Canada</citizenship>
            <age>22</age>
        </student>
    </students>

    We can construct a XPath query and use it to search.


    Args:
        xml_data: The XML data containing the student information.
        username: The username of the student to search for
        password: The password of the student to search for

    Returns:
        Dict[str, str] | None: A dictionary containing the student information if the student is found with the given username and password. Otherwise, return None.
    """

    # Parse the XML data
    root = etree.fromstring(xml_data)

    # Construct an XPath query to find the student with the given username and password
    xpath_query = f".//student[username='{username}' and password='{password}']"

    # Find the student element that matches the XPath query
    student_element = root.xpath(xpath_query)

    # If no matching student is found, return None
    if not student_element:
        return None

    # Extract the student information from the student element
    student_info = {
        "name": student_element[0].find("name").text,
        "citizenship": student_element[0].find("citizenship").text,
        "age": student_element[0].find("age").text,
    }

    # Return the student information as a dictionary
    return student_info