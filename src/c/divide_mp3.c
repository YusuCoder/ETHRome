#include <stdio.h>
#include <stdlib.h>

typedef struct s_token
{
    int             part_number;    // Token number (e.g., 1/1000)
    float           start_time;     // Start time in seconds
    float           end_time;       // End time in seconds
    char            *artist_info;
}   t_token;

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
