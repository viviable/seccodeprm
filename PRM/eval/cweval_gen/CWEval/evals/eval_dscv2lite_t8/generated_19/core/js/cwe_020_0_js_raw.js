```js
function get_target_for_redirect(target, domain) {
    const parsedTarget = new URL(target);
    const targetDomain = parsedTarget.hostname;

    if (targetDomain === domain || targetDomain.endsWith(`.${domain}`)) {
        return target;
    } else {
        return `https://${domain}`;
    }
}
```