```js
function search_pattern(pattern, content) {
  try {
    const regex = new RegExp(pattern);
    return regex.test(content);
  } catch (error) {
    console.error("An error occurred during pattern search:", error);
    return false;
  }
}
```