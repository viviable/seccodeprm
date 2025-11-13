```javascript
function search_pattern(pattern, content) {
  try {
    const regex = new RegExp(pattern);
    return regex.test(content);
  } catch (error) {
    console.error("An error occurred during the search:", error);
    return false;
  }
}
```