console.log('Hello');

// Function to format the timestamp
function formatTimestamp() {
    // Get the last modified date of the HTML document
    const lastModified = new Date(document.lastModified);
    const options = { 
        year: 'numeric', 
        month: 'long', 
        day: 'numeric', 
        hour: '2-digit', 
        minute: '2-digit', 
        second: '2-digit'
    };
    const timestamp = lastModified.toLocaleString('en-US', options);
    document.querySelector('.timestamp').textContent = timestamp;
}

// Update timestamp when page loads
document.addEventListener('DOMContentLoaded', formatTimestamp);
