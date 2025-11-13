```js
function getUrlForQuery(apiUrl, userId) {
    if (!userId || typeof userId !== 'string' || userId.trim() === '') {
        return null;
    }
    return `${apiUrl}/${userId}`;
}
```