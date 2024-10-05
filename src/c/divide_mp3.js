const fs = require('fs');

class Token {
    constructor(part_number, start_time, end_time, artist_info) {
        this.part_number = part_number;
        this.start_time = start_time;
        this.end_time = end_time;
        this.artist_info = artist_info || 'Unknown Artist';
    }
}

async function divideMp3(mp3File, artistInfo) {
    try {
        // Dynamically import the music-metadata module
        const mm = await import('music-metadata');

        // Read the MP3 file and extract metadata, including duration
        const metadata = await mm.parseFile(mp3File);
        const songDuration = metadata.format.duration; // Duration in seconds

        console.log(`songDuration: ${songDuration}\n`);

        if (!songDuration) {
            throw new Error("Unable to retrieve the song's duration.");
        }

        const partDuration = songDuration / 1000.0;
        const tokens = [];

        // Loop through and create 1000 tokens
        for (let i = 0; i < 1000; i++) {
            let token = new Token(
                i + 1,                   // part_number
                i * partDuration,         // start_time
                (i + 1) * partDuration,   // end_time
                artistInfo                // artist_info
            );
            tokens.push(token);
        }

        // // Print out metadata for verification
        // tokens.forEach((token) => {
        //     console.log(`Token ${token.part_number}: Start time = ${token.start_time.toFixed(2)} seconds, End time = ${token.end_time.toFixed(2)} seconds`);
        // });

        return tokens;

    } catch (err) {
        console.error("Error:", err.message);
        return [];
    }
}

// Main execution
(async function main() {
    const mp3File = 'i_will.mp3';
    const artistInfo = 'Example Artist';

    const tokens = await divideMp3(mp3File, artistInfo);
})();

// npm install fs
// npm install music-metadata
