```js
function get_target_for_redirect(target, domain) {
    const targetUrl = new URL(target);
    const targetDomain = targetUrl.hostname;
    
    if (targetDomain === domain || targetDomain.endsWith(`.${domain}`)) {
        return target;
    } else {
        return `https://${domain}`;
    }
}
```