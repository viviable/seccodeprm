const { JSDOM } = require('jsdom');
const xpath = require('xpath');

/**
 * Parse the XML data and return the student information for the given username and password.
 *
 * @param {string} xml_data - The XML data containing the student information.
 * @param {string} username - The username of the student to search for.
 * @param {string} password - The password of the student to search for.
 * @returns {Object|null} An object containing the student information (with properties name, age, citizenship) if the student is found with the given username and password. Otherwise, return null.
 */
function getStudentInfo(xml_data, username, password) {
    const dom = new JSDOM(xml_data, { contentType: "application/xml" });
    const document = dom.window.document;

    // Construct an XPath query to find the student node with the matching username and password
    const xpathQuery = `//student[username='${username}' and password='${password}']`;

    // Evaluate the XPath query
    const nodes = xpath.select(xpathQuery, document);

    // Check if any nodes were found
    if (nodes.length > 0) {
        const studentNode = nodes[0];
        const name = studentNode.querySelector('name').textContent;
        const age = studentNode.querySelector('age').textContent;
        const citizenship = studentNode.querySelector('citizenship').textContent;

        return { name, age, citizenship };
    }

    // Return null if no matching student was found
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
