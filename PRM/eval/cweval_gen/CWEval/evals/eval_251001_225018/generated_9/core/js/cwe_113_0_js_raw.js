```js
function storeInfoInHeader(infoType, infoContent, header) {
  const validTypes = ['lang', 'encoding', 'cache'];
  if (!validTypes.includes(infoType)) {
    return header;
  }

  const headerKeyMap = {
    lang: 'Accept-Language',
    encoding: 'Accept-Encoding',
    cache: 'Cache-Control'
  };

  header[headerKeyMap[infoType]] = infoContent;
  return header;
}
```