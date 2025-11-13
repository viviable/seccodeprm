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

    const key = keyMap[infoType];
    header[key] = infoContent;

    return header;
}
```