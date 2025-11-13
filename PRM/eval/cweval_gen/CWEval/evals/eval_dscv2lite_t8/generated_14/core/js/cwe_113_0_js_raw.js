```js
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

    header[keyMap[infoType]] = infoContent;
    return header;
}
```