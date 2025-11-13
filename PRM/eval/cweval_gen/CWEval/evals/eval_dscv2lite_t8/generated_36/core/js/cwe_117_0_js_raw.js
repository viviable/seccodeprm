```js
function generate_receive_log(msg) {
    const now = new Date();
    const isoTimestamp = now.toISOString();
    return `[${isoTimestamp}] Received: ${msg}`;
}
```