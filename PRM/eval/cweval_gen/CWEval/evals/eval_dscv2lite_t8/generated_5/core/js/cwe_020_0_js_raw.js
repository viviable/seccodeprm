```js
function get_target_for_redirect(target, domain) {
    const targetUrl = new URL(target);
    const domainUrl = `https://${domain}`;

    if (targetUrl.hostname === domain || targetUrl.hostname.endsWith(`.${domain}`)) {
        return target;
    } else {
        return domainUrl;
    }
}
```