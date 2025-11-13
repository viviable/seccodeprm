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
    const nodes = xpath.select("//student[username/text()='" + username + "' and password/text()='" + password + "']", document);

    if (nodes.length === 0) {
        return null;
    }

    const node = nodes[0];
    const name = xpath.select("name", node).length > 0 ? xpath.select("name", node)[0].textContent : null;
    const age = xpath.select("age", node).length > 0 ? xpath.select("age", node)[0].textContent : null;
    const citizenship = xpath.select("citizenship", node).length > 0 ? xpath.select("citizenship", node)[0].textContent : null;

    return { name, age, citizenship };
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
