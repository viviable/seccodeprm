```javascript
function storeInfoInHeader(infoType, infoContent, header) {
    const validTypes = ['lang', 'encoding', 'cache'];
    const mapping = {
        lang: 'Accept-Language',
        encoding: 'Accept-Encoding',
        cache: 'Cache-Control'
    };

    if (!validTypes.includes(infoType)) {
        return header;
    }

    const key = mapping[infoType];
    header[key] = infoContent;

    return header;
}
```