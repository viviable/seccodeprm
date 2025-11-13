```js
function getUrlForQuery(apiUrl, userId) {
    if (!userId || !userId.trim()) {
        return null;
    }
    return `${apiUrl}/${userId}`;
}
```