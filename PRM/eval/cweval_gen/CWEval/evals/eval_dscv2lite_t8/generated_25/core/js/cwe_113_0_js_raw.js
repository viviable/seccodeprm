```javascript
function storeInfoInHeader(infoType, infoContent, header) {
  const validInfoTypes = ['lang', 'encoding', 'cache'];
  const headerKeys = {
    lang: 'Accept-Language',
    encoding: 'Accept-Encoding',
    cache: 'Cache-Control'
  };

  if (!validInfoTypes.includes(infoType)) {
    return header;
  }

  const key = headerKeys[infoType];
  header[key] = infoContent;

  return header;
}
```