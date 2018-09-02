// ADPCM HEADER FILE created by Neo Sound Builder

inline void send_sound_command(BYTE CommandNo)
{
    (*((PBYTE)0x320000)) = CommandNo;
}

// ADPCM System commands
#define ADPCM_SET_MAX_VOL (0x0b)
#define ADPCM_DEC_VOL (0x0c)
#define ADPCM_INC_VOL (0x0d)
#define ADPCM_FADE_OUT (0x0e)
#define ADPCM_OFF (0x0f)

// ADPCM Sample commands
#define ADPCM_BASS2 (0x10)
#define ADPCM_CLOSEDHH2 (0x11)
#define ADPCM_OPENHH2 (0x12)
#define ADPCM_SNARE2 (0x13)
#define ADPCM_JEROEN_TEL___CYBERNOID_II (0x14)

//Sample name array
#define SAMPLE_COUNT 6
static const char SoundList[5][37] =
{
    "BASS2                               \0", 
    "CLOSEDHH2                           \0", 
    "OPENHH2                             \0", 
    "SNARE2                              \0", 
    "JEROEN_TEL___CYBERNOID_II           \0" 
};
