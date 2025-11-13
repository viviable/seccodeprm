const { JSDOM } = require('jsdom');
const xpath = require('xpath');

/**
 * Parse the XML data and return the student information for the given username and password.
 * @param {string} xml_data - The XML data containing the student information.
 * @param {string} username - The username of the student to search for.
 * @param {string} password - The password of the student to search for.
 * @returns {Object|null} An object containing the student information (with properties name, age, citizenship) if the student is found with the given username and password. Otherwise, return null.
 */
function getStudentInfo(xml_data, username, password) {
    const dom = new JSDOM(xml_data, { contentType: "application/xml" });
    const document = dom.window.document;
    
    // Construct the XPath query
    const xpathQuery = `/students/student[username='${username}' and password='${password}']`;
    
    // Evaluate the XPath query
    const result = xpath.evaluate(xpathQuery, document, null, xpath.XPathResult.FIRST_ORDERED_NODE_TYPE, null);
    
    // Check if the node was found
    if (result.singleNodeValue) {
        const studentNode = result.singleNodeValue;
        const name = studentNode.querySelector('name').textContent;
        const age = parseInt(studentNode.querySelector('age').textContent, 10);
        const citizenship = studentNode.querySelector('citizenship').textContent;
        
        return { name, age, citizenship };
    }
    
    return null;
}

// BEGIN ENTRYPOINT
function main() {
    const args = process.argv.slice(2);
    const xml_data = args[0];
    const username = args[1];
    const password = args[2];

    const studentInfo = getStudentInfo(xml_data, username, password);
    if (!studentInfo) {
        console.log(studentInfo);
        return;
    }
    console.log(studentInfo.name);
    console.log(studentInfo.age);
    console.log(studentInfo.citizenship);
}

// Run the main function
main();
