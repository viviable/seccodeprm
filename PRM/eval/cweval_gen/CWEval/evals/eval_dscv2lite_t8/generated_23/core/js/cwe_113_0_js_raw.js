```js
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
```