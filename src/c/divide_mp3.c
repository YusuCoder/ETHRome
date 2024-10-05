#include <stdio.h>
#include <stdlib.h>

typedef struct s_token
{
    int             part_number;    // Token number (e.g., 1/1000)
    float           start_time;     // Start time in seconds
    float           end_time;       // End time in seconds
    unsigned char   *data;          // Binary data (1/1000 of the song)
    int             data_size;      // Size of the binary data in bytes
}   t_token;

int divide_mp3(char *mp3_file, t_token  *tokens)
{
    FILE    *mp3File;
    long    fileSize;
    int     partSize, remainder, i;
    float   songDuration = 240.0;  // duration in seconds (example: 4 minutes)
    float   partDuration = songDuration / 1000.0;

    // Open the input MP3/WAV file in binary mode
    mp3File = fopen(mp3_file, "rb");
    if (!mp3File) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

    // Find the size of the file
    fseek(mp3File, 0, SEEK_END);
    fileSize = ftell(mp3File);
    fseek(mp3File, 0, SEEK_SET);

    // Calculate the size of each part in bytes
    partSize = fileSize / 1000;
    remainder = fileSize % 1000;

    // Loop through and create 1000 parts, storing metadata and binary data
    for (i = 0; i < 1000; i++) {
        // Prepare token metadata
        tokens[i].part_number = i + 1;
        tokens[i].start_time = i * partDuration;
        tokens[i].end_time = (i + 1) * partDuration;

        // Allocate memory for the binary data of this token
        tokens[i].data = (unsigned char*)malloc(partSize);
        if (!tokens[i].data) {
            printf("Error: Unable to allocate memory for token %d.\n", i + 1);
            fclose(mp3File);
            return 1;
        }

        // Read the binary data (1/1000 of the file) from the MP3 file
        tokens[i].data_size = fread(tokens[i].data, 1, partSize, mp3File);

        if (tokens[i].data_size != partSize) {
            printf("Error reading binary data for token %d.\n", i + 1);
            free(tokens[i].data);
            fclose(mp3File);
            return 1;
        }
    }

    // Handle the remainder (extra bytes) if any
    if (remainder > 0) {
        tokens[i].part_number = 1001;
        tokens[i].start_time = 1000 * partDuration;
        tokens[i].end_time = songDuration;
        tokens[i].data = (unsigned char*)malloc(remainder);
        tokens[i].data_size = fread(tokens[i].data, 1, remainder, mp3File);
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
    t_token tokens[1001];
    char    *mp3_file = "i_will.mp3";

    divide_mp3(mp3_file, tokens);
}
