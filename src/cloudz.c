#include "include/cloud.h"
#include "include/util.h"

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
#define ROTATEX "rotatex"
#define ROTATEY "rotatey"
#define ROTATEZ "rotatez"

void greetings()
{
	printf("cloudz: Utilitario Manipulador de Nuvens de Pontos\n");
	printf("Artur Rodrigues Rocha Neto @INTERFACES/UFC - c2018\n");
	printf("[!!] digite help para ver a lista de comandos [!!]\n\n");
}

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
	printf("%s <degrees:angle>\n", ROTATEX);
	printf("%s <degrees:angle>\n", ROTATEY);
	printf("%s <degrees:angle>\n", ROTATEZ);
	printf("\n");
}

void sort(struct cloud* cloud)
{
	cloud_sort(cloud);
	printf("nuvem ordenada\n");
}

void center(struct cloud* cloud)
{
	vector3_debug(stdout, cloud_get_center(cloud));
}

void size(struct cloud* cloud)
{
	printf("%d\n", cloud_size(cloud));
}

void save(struct cloud* cloud, const char* param)
{
	if (cloud_save_xyz(cloud, param))
		printf("nuvem salva em %s\n", param);
}

void scale(struct cloud* cloud, const char* param)
{
	real f;
	
	if (sscanf(param, "%lf", &f) < 1) {
		util_error("parametros incorretos para comando scale");
	} else {
		cloud_scale(cloud, f);
		printf("escalamento de %.4fx concluido\n", f);
	}
}

void translate(struct cloud* cloud, const char* param)
{
	real x;
	real y;
	real z;
	
	if (sscanf(param, "%lf %lf %lf", &x, &y, &z) < 3) {
		util_error("parametros incorretos para comando translate");
	} else {
		cloud_translate_real(cloud, x, y, z);
		printf("nuvem movida para (%.2f, %.2f, %.2f)\n", x, y, z);
	}
}

void rotatex(struct cloud* cloud, const char* param)
{
	real d;
	
	if (sscanf(param, "%lf", &d) < 1) {
		util_error("parametros incorretos para comando rotatex");
	} else {
		cloud_rotate_x(cloud, d);
		printf("nuvem rotacionada em torno de x por %.2fº\n", d);
	}
}

void rotatey(struct cloud* cloud, const char* param)
{
	real d;
	
	if (sscanf(param, "%lf", &d) < 1) {
		util_error("parametros incorretos para comando rotatey");
	} else {
		cloud_rotate_y(cloud, d);
		printf("nuvem rotacionada em torno de y por %.2fº\n", d);
	}
}

void rotatez(struct cloud* cloud, const char* param)
{
	real d;
	
	if (sscanf(param, "%lf", &d) < 1) {
		util_error("parametros incorretos para comando rotatez");
	} else {
		cloud_rotate_z(cloud, d);
		printf("nuvem rotacionada em torno de z por %.2fº\n", d);
	}
}

/******************************************************************************/

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
		sort(cloud);
	} else if (!strcmp(command, CENTER)) {
		center(cloud);
	} else if (!strcmp(command, SIZE)) {
		size(cloud);
	} else if (!strcmp(command, SAVE)) {
		save(cloud, param);
	} else if (!strcmp(command, SCALE)) {
		scale(cloud, param);
	} else if (!strcmp(command, TRANSLATE)) {
		translate(cloud, param);
	} else if (!strcmp(command, ROTATEX)) {
		rotatex(cloud, param);
	} else if (!strcmp(command, ROTATEY)) {
		rotatey(cloud, param);
	} else if (!strcmp(command, ROTATEZ)) {
		rotatez(cloud, param);
	} else {
		util_error("comando %s desconhecido!", command);
	}
	
	return running;
}

void cli(int argc, char** argv, struct cloud* cloud)
{
	char* command = argv[2];
	char param[PARAM_SIZE];
	
	memset(param, 0, PARAM_SIZE);
	for (int i = 3; i < argc; i++) {
		strcat(param, argv[i]);
		if (i+1 < argc)
			strcat(param, " ");
	}
	
	if(!strcmp(command, QUIT)) {
		return;
	} else if (!strcmp(command, HELP)) {
		help();
	} else if (!strcmp(command, SORT)) {
		sort(cloud);
	} else if (!strcmp(command, CENTER)) {
		center(cloud);
	} else if (!strcmp(command, SIZE)) {
		size(cloud);
	} else if (!strcmp(command, SAVE)) {
		save(cloud, param);
	} else if (!strcmp(command, SCALE)) {
		scale(cloud, param);
	} else if (!strcmp(command, TRANSLATE)) {
		translate(cloud, param);
	} else if (!strcmp(command, ROTATEX)) {
		rotatex(cloud, param);
	} else if (!strcmp(command, ROTATEY)) {
		rotatey(cloud, param);
	} else if (!strcmp(command, ROTATEZ)) {
		rotatez(cloud, param);
	} else {
		util_error("comando %s desconhecido!", command);
	}
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		util_error("numero incorreto de argumentos");
		util_info("uso: cloudz <arquivo_nuvem> <!comando> <!parametros>");
		util_info("<comando> e <parametros> sao opcionais");
		
		exit(1);
	} else if (argc > 2) {
		struct cloud* cloud = cloud_load_xyz(argv[1]);
		cli(argc, argv, cloud);
		cloud_free(cloud);
		
		return 0;
	}
	
	greetings();
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	if (cloud == NULL)
		exit(1);
	
	while(prompt(cloud));
	cloud_free(cloud);
	
	return 0;
}

