```js
function storeInfoInHeader(infoType, infoContent, header) {
  const validInfoTypes = ['lang', 'encoding', 'cache'];
  if (!validInfoTypes.includes(infoType)) {
    return header;
  }

  const keyMap = {
    lang: 'Accept-Language',
    encoding: 'Accept-Encoding',
    cache: 'Cache-Control'
  };

  const key = keyMap[infoType];
  header[key] = infoContent;

  return header;
}
```