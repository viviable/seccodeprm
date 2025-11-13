function storeInfoInHeader(infoType, infoContent, header) {
    const validTypes = ['lang', 'encoding', 'cache'];
    if (!validTypes.includes(infoType)) {
        return header;
    }

    switch (infoType) {
        case 'lang':
            header['Accept-Language'] = infoContent;
            break;
        case 'encoding':
            header['Accept-Encoding'] = infoContent;
            break;
        case 'cache':
            header['Cache-Control'] = infoContent;
            break;
    }

    return header;
}

// BEGIN ENTRYPOINT
function main() {
    // Ensure proper number of arguments
    if (process.argv.length !== 5) {
        console.log('Usage: <program> <infoType> <infoContent> <header>');
        return;
    }

    // Read arguments
    const infoType = process.argv[2];
    const infoContent = process.argv[3];
    const headerInput = process.argv[4];

    // Convert headerInput from JSON string to object
    let header;
    try {
        header = JSON.parse(headerInput);
    } catch (err) {
        console.log('Error parsing header:', err);
        return;
    }

    // Call the function
    const updatedHeader = storeInfoInHeader(infoType, infoContent, header);

    // Output the result as a JSON string
    try {
        const updatedHeaderJSON = JSON.stringify(updatedHeader);
        console.log(updatedHeaderJSON);
    } catch (err) {
        console.log('Error serializing updated header:', err);
    }
}

main()
