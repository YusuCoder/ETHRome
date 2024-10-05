#include <stdio.h>
#include <stdlib.h>

typedef struct s_token
{
    int             part_number;    // Token number (e.g., 1/1000)
    float           start_time;     // Start time in seconds
    float           end_time;       // End time in seconds
    char            *artist_info;
}   t_token;

#include <stdio.h>
#include <stdlib.h>

float get_song_duration(const char *mp3_file)
{
    FILE *fp = fopen(mp3_file, "rb");
    if (!fp) {
        printf("Error: Unable to open file %s\n", mp3_file);
        return 0.0;
    }

    // Calculate file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    long totalBits = 0;     // Total bits processed
    int totalFrames = 0;    // Total frames processed

    // Read until the end of the file
    unsigned char header[4];
    while (fread(header, 1, 4, fp) == 4) {
        // Check for valid MP3 frame sync (11 bits of 1s)
        if (header[0] == 0xFF && (header[1] & 0xE0) == 0xE0) {
            // Extract bitrate (bits 12-15) and sample rate (bits 10-11)
            int bitrateIndex = (header[2] >> 4) & 0x0F;  // bits 12-15
            int sampleRateIndex = (header[2] >> 2) & 0x03; // bits 10-11

            // Bitrates according to MPEG-1 Layer III
            int bitrates[] = {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320};
            int sampleRates[] = {44100, 48000, 32000}; // in Hz

            // Get bitrate and sample rate
            int bitrate = (bitrateIndex < sizeof(bitrates) / sizeof(bitrates[0])) ? bitrates[bitrateIndex] * 1000 : 0;
            int sampleRate = (sampleRateIndex < sizeof(sampleRates) / sizeof(sampleRates[0])) ? sampleRates[sampleRateIndex] : 0;

            // If bitrate or sample rate is not valid, skip this frame
            if (bitrate == 0 || sampleRate == 0) {
                continue;
            }

            // Frame length is calculated based on the bitrate and sample rate
            int frameLength = 144 * bitrate / sampleRate + (header[2] & 0x02 ? 1 : 0); // Add 1 for padding if present
            totalBits += bitrate; // Total bits for this frame
            totalFrames++; // Increment frame count

            // Move the file pointer to the next frame
            fseek(fp, frameLength - 4, SEEK_CUR); // -4 because we already read 4 bytes of the header
        } else {
            // Not a valid frame, continue reading
            fseek(fp, -3, SEEK_CUR); // Move back to check for the next possible frame
        }
    }

    fclose(fp);

    if (totalFrames == 0) {
        printf("Error: No valid MP3 frames found\n");
        return 0.0;
    }

    // Average bitrate calculation
    float averageBitrate = (float)totalBits / totalFrames;

    // Calculate duration in seconds
    float duration = (fileSize * 8.0) / averageBitrate; // Convert bytes to bits

    // Check for division by zero or negative values
    if (duration < 0) {
        printf("Error: Calculated duration is negative, check bitrate and frame calculations.\n");
        return 0.0;
    }
    return duration;
}



int divide_mp3(char *mp3_file, t_token  *tokens)
{
    FILE    *mp3File;
    float   songDuration = 240.0;  // duration in seconds (example: 4 minutes)
    float   partDuration = songDuration / 1000.0;
    int     i;

    // Open the input MP3/WAV file in binary mode
    mp3File = fopen(mp3_file, "rb");
    if (!mp3File) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

<<<<<<< HEAD
songDuration = get_song_duration(mp3_file);

    // Find the size of the file
    fseek(mp3File, 0, SEEK_END);
    fileSize = ftell(mp3File);
    fseek(mp3File, 0, SEEK_SET);

    // Calculate the size of each part in bytes
    partSize = fileSize / 1000;
    remainder = fileSize % 1000;

=======
>>>>>>> ea9c73dcb16e5278f467311a80066c942aeba739
    // Loop through and create 1000 parts, storing metadata and binary data
    for (i = 0; i < 1000; i++) {
        // Prepare token metadata
        tokens[i].part_number = i + 1;
        tokens[i].start_time = i * partDuration;
        tokens[i].end_time = (i + 1) * partDuration;
    }

    // // Output metadata and binary data for verification
    // for (i = 0; i < 1000; i++) {
    //     printf("Token %d: Start time = %.2f seconds, End time = %.2f seconds, Data size = %d bytes\n",
    //             tokens[i].part_number, tokens[i].start_time, tokens[i].end_time, tokens[i].data_size);
    // }

    fclose(mp3File);

    return 0;
}

int main(void)
{
    t_token tokens[1000];
    char    *mp3_file = "i_will.mp3";

    divide_mp3(mp3_file, tokens);
}
