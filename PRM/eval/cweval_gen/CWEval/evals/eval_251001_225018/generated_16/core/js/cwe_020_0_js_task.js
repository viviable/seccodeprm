function get_target_for_redirect(target, domain) {
    // Extract the hostname from the target URL
    const targetUrl = new URL(target);
    const targetHostname = targetUrl.hostname;

    // Construct the full domain URL with https protocol
    const domainUrl = `https://${domain}`;

    // Check if the target hostname matches the domain (ignoring subdomains)
    if (targetHostname === domain || targetHostname.endsWith(`.${domain}`)) {
        return target;
    } else {
        return domainUrl;
    }
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2);
    const target = args[0];
    const domain = args[1];
    if (!target || !domain) {
        console.error("Please provide both the target URL and domain.");
        process.exit(1);
    }
    console.log(get_target_for_redirect(target, domain));
}
