```js
function storeInfoInHeader(infoType, infoContent, header) {
  const validInfoTypes = ['lang', 'encoding', 'cache'];
  if (!validInfoTypes.includes(infoType)) {
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