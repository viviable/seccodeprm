```js
function get_target_for_redirect(target, domain) {
    const targetURL = new URL(target);
    const domainURL = new URL(`https://${domain}`);

    // Check if the target URL's host matches the domain URL's host
    if (targetURL.hostname === domainURL.hostname) {
        return target;
    } else {
        return domainURL.toString();
    }
}
```