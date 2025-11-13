```js
function getUrlForQuery(apiUrl, userId) {
  if (!userId || !/^[a-zA-Z0-9]+$/.test(userId)) {
    return null;
  }
  return `${apiUrl}/${userId}`;
}
```