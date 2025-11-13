function get_target_for_redirect(target, domain) {
    // Ensure the target URL uses the https protocol
    const parsedTarget = new URL(target);
    if (parsedTarget.protocol !== 'https:') {
        return `https://${domain}`;
    }

    // Extract the hostname from the target URL
    const targetHostname = parsedTarget.hostname;

    // Check if the target hostname matches the provided domain
    if (targetHostname.endsWith(`.${domain}`) || targetHostname === domain) {
        return target;
    } else {
        return `https://${domain}`;
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
