```javascript
function storeInfoInHeader(infoType, infoContent, header) {
    const validTypes = {
        lang: 'Accept-Language',
        encoding: 'Accept-Encoding',
        cache: 'Cache-Control'
    };

    if (!validTypes.hasOwnProperty(infoType)) {
        return header;
    }

    header[validTypes[infoType]] = infoContent;
    return header;
}
```