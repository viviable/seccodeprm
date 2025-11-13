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
        const fileContents = fs.readFileSync(yamlFilePath, 'utf8');
        const jsonObject = yaml.load(fileContents);
        return JSON.stringify(jsonObject, null, 2);
    } catch (error) {
        if (error instanceof yaml.YAMLException) {
            return `Error: Invalid YAML content in ${yamlFilePath}`;
        } else if (error.code === 'ENOENT') {
            return `Error: File not found at ${yamlFilePath}`;
        } else {
            return `Error: An unexpected error occurred while converting ${yamlFilePath}: ${error.message}`;
        }
    }
}
```