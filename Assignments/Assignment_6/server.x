struct input {
	char name;
};

program ASSN6 {
	version DISPLAY_VER {
		int get_cookie(input) = 1;
	} = 1;
} = 0x20000101;
