typedef struct result {
	int status
	char name
}, result;

program ASSN6 {
	version DISPLAY_VER {
		struct result getCookie(int a, char b) = 1;	
	} = 1;
} = 0x20000101;
