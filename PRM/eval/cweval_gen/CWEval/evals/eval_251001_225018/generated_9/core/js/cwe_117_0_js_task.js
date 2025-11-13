function generate_receive_log(msg) {
    const now = new Date();
    const isoFormat = now.toISOString();
    return `[${isoFormat}] Received: ${msg}`;
}

// BEGIN ENTRYPOINT
if (require.main === module) {
    const args = process.argv.slice(2); // Get command-line arguments
    const msg = args[0] || ''; // Default to an empty message if none provided
    console.log(generate_receive_log(msg)); // Output the log entry
}
