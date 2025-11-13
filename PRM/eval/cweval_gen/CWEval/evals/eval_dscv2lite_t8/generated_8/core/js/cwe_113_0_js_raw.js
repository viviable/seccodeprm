```js
function storeInfoInHeader(infoType, infoContent, header) {
  const validInfoTypes = ['lang', 'encoding', 'cache'];
  if (!validInfoTypes.includes(infoType)) {
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