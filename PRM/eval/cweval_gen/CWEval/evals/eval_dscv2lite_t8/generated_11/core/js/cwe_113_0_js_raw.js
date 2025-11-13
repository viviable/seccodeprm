```js
function storeInfoInHeader(infoType, infoContent, header) {
    const validTypes = ['lang', 'encoding', 'cache'];
    if (!validTypes.includes(infoType)) {
        return header;
    }
    
    const keyMap = {
        lang: 'Accept-Language',
        encoding: 'Accept-Encoding',
        cache: 'Cache-Control'
    };
    
    header[keyMap[infoType]] = infoContent;
    return header;
}
```