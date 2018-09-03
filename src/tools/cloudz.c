#include "../include/cloud.h"
#include "../include/util.h"

#define INPUT_SIZE 80
#define COMMAND_SIZE 16
#define PARAM_SIZE 64

#define QUIT "quit"
#define HELP "help"
#define SORT "sort"
#define CENTER "center"
#define SIZE "size"
#define SAVE "save"
#define SCALE "scale"
#define TRANSLATE "translate"
#define ROTATE "rotate"

void help()
{
	printf("\n");
	printf("%s\n", QUIT);
	printf("%s\n", HELP);
	printf("%s\n", SORT);
	printf("%s\n", CENTER);
	printf("%s\n", SIZE);
	printf("%s <string:filepath>\n", SAVE);
	printf("%s <real:size>\n", SCALE);
	printf("%s <real:x> <real:y> <real:z>\n", TRANSLATE);
	printf("%s <degrees:angle>\n", ROTATE);
	printf("\n");
}

int prompt(struct cloud* cloud)
{
	int running = 1;
	
	char input[INPUT_SIZE];
	char command[COMMAND_SIZE];
	char param[PARAM_SIZE];
	
	printf("> ");
	fgets(input, INPUT_SIZE, stdin);
	if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n'))
		input[strlen(input) - 1] = '\0';
	
	sscanf(input, "%s %[^\t\n]", command, param);
	
	if(!strcmp(command, QUIT)) {
		running = 0;
	} else if (!strcmp(command, HELP)) {
		help();
	} else if (!strcmp(command, SORT)) {
		cloud_sort(cloud);
		printf("sort done\n");
	} else if (!strcmp(command, CENTER)) {
		struct vector3* center = cloud_get_center(cloud);
		vector3_debug(stdout, center);
	} else if (!strcmp(command, SIZE)) {
		printf("%d\n", cloud_size(cloud));
	} else if (!strcmp(command, SAVE)) {
		if (cloud_save_csv(cloud, param))
			printf("cloud saved at %s\n", param);
	} else if (!strcmp(command, SCALE)) {
		real f;
		sscanf(param, "%lf", &f);
		cloud_scale(cloud, f);
		printf("scale done\n");
	} else if (!strcmp(command, TRANSLATE)) {
		// asdf
	} else if (!strcmp(command, ROTATE)) {
		// asdf
	} else {
		util_error("command %s not found", command);
	}
	
	return running;
}

int main(int argc, char** argv)
{
	util_prompt("<<_cloudZ_>> - Artur@INTERFACES -");
	
	if (argc != 2) {
		util_error("numero incorreto de argumentos");
		util_info("uso: cloudz <arquivo_nuvem>");
		exit(1);
	}
	
	struct cloud* cloud = cloud_load_csv(argv[1]);
	if (cloud == NULL) {
		util_error("nao foi possivel iniciar o programa");
		exit(1);
	}
	
	while(prompt(cloud));
	
	cloud_free(cloud);
	
	return 0;
}

