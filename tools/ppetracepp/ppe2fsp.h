

#define P2F_NULL_POINTER                1
#define P2F_INVALID_INPUT_SIZE          2
#define P2F_INVALID_PPE_OFFSET          3
#define P2F_OUTPUT_BUFFER_TOO_SMALL     4
#define P2F_INPUT_BUFFER_TOO_SMALL      5
#define P2F_INVALID_VERSION             6

int ppe2fsp(void* in, unsigned long in_size, void* out, unsigned long* io_size);
