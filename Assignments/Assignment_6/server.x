struct input {
	int a;
	char b;
};

struct result {
	int status;
	char name;
};

program ASSN6 {
	version DISPLAY_VER {
		struct result getCookie(input) = 1;	
	} = 1;
} = 0x20000101;
