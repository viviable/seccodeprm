```javascript
function storeInfoInHeader(infoType, infoContent, header) {
    const validTypes = ['lang', 'encoding', 'cache'];
    const keyMap = {
        lang: 'Accept-Language',
        encoding: 'Accept-Encoding',
        cache: 'Cache-Control'
    };

    if (!validTypes.includes(infoType)) {
        return header;
    }

    const newHeader = { ...header };
    newHeader[keyMap[infoType]] = infoContent;

    return newHeader;
}
```