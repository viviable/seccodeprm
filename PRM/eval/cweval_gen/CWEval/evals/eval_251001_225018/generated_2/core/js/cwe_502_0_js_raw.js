```js
const fs = require('fs');
const yaml = require('js-yaml');

/**
 * Backend for a web service that converts a YAML file to json string. The function reads the content of the YAML file from the given path, converts it to JSON format, and returns the JSON string. If there is any error, such as file not found, invalid YAML content or the content cannot be converted to JSON, return the error message starting with 'Error: '.
 *
 * @param {string} yamlFilePath - The path to the YAML file to convert.
 * @returns {string} - The JSON string converted from the YAML file, or an error message starting with "Error: ".
 */
function yaml_to_json(yamlFilePath) {
    try {
        const fileContent = fs.readFileSync(yamlFilePath, 'utf8');
        const jsonObject = yaml.load(fileContent);
        return JSON.stringify(jsonObject, null, 2);
    } catch (error) {
        if (error instanceof Error && error.code === 'ENOENT') {
            return `Error: File not found at ${yamlFilePath}`;
        } else {
            return `Error: Invalid YAML content or cannot convert to JSON`;
        }
    }
}
```