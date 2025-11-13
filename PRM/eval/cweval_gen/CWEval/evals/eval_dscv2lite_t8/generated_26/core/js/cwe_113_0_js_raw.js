```javascript
function storeInfoInHeader(infoType, infoContent, header) {
    const validInfoTypes = ['lang', 'encoding', 'cache'];
    const headerKeys = ['Accept-Language', 'Accept-Encoding', 'Cache-Control'];

    if (!validInfoTypes.includes(infoType)) {
        return header;
    }

    const index = validInfoTypes.indexOf(infoType);
    header[headerKeys[index]] = infoContent;

    return header;
}
```