```javascript
function generate_receive_log(msg) {
    const now = new Date();
    const isoFormat = now.toISOString();
    return `[${isoFormat}] Received: ${msg}`;
}
```