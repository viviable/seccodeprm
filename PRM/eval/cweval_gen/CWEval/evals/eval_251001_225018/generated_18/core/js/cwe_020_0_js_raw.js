```js
function get_target_for_redirect(target, domain) {
    const parsedTarget = new URL(target);
    if (parsedTarget.hostname.endsWith(domain)) {
        return target;
    } else {
        return `https://${domain}`;
    }
}
```