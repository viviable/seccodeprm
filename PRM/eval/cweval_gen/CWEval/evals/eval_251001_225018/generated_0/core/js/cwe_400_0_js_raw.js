```js
function search_pattern(pattern, content) {
  try {
    const regex = new RegExp(pattern);
    return regex.test(content);
  } catch (e) {
    console.error("An error occurred during the search:", e);
    return false;
  }
}
```