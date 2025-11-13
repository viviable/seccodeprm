```js
function storeInfoInHeader(infoType, infoContent, header) {
    const validTypes = ['lang', 'encoding', 'cache'];
    if (!validTypes.includes(infoType)) {
        return header;
    }

    const headerKeys = {
        lang: 'Accept-Language',
        encoding: 'Accept-Encoding',
        cache: 'Cache-Control'
    };

    header[headerKeys[infoType]] = infoContent;
    return header;
}
```