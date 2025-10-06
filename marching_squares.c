//Mihut Maria-Emilia, 311CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int r, g, b;
	int l;// luminozitate
} pixel;

pixel **read(pixel **m, int *w, int *h, int *exit);
void write(pixel **m, int w, int h);
pixel **resize(pixel **m, int *w, int *h);
void free_image(pixel **m, int h);
int **create_grid(pixel **m, int w, int h);
void march(pixel **m, int w, int h);
void init_contur(int contur[16][16]);
void print_contur(int contur[16][16]);

int main(void)
{
	pixel **m = NULL;//matricea de pixeli
	int w = 0, h = 0, exit = 0;//width and height, exit cand e gata
	char command[20];
	int contur[16][16];
	while (!exit) {
		scanf("%s", command);
		if (strcmp(command, "READ") == 0) {
			if (m) {
				free_image(m, h);
			}
			m = read(m, &w, &h, &exit);
			if (exit) {
				break;
			}
		} else if (strcmp(command, "WRITE") == 0) {
			if (m) {
				write(m, w, h);
			}
		} else if (strcmp(command, "RESIZE") == 0) {
			if (m) {
				m = resize(m, &w, &h);
			}
		} else if (strcmp(command, "GRID") == 0) {
			if (m) {
				int **grid = create_grid(m, w, h);
				printf("Grid calculat cu succes [%d-%d]\n", h / 4, w / 4);
				for (int i = 0; i < h / 4; i++) {
					for (int j = 0; j < w / 4; j++) {
						printf("%d ", grid[i][j]);
					}
					printf("\n");
				}
				for (int i = 0; i < h / 4 + 1; i++) {
					free(grid[i]);
				}
				free(grid);
			}
		} else if (strcmp(command, "MARCH") == 0) {
			if (m) {
				march(m, w, h);
			}
		} else if (strcmp(command, "INIT_CONTUR") == 0) {
			init_contur(contur);
			for (int i = 0; i < 16; i++) {
				printf("P3\n4 4\n255\n");//standard
				for (int j = 0; j < 16; j++) {
					int x = contur[i][j];
					printf("%d %d %d\n", x, x, x);
				}
			}
		} else if (strcmp(command, "EXIT") == 0) {
			printf("Gigel a terminat\n");
			if (m) {
				free_image(m, h);
			}
			exit = 1;
		}
	}
	return 0;
}

pixel **read(pixel **m, int *w, int *h, int *exit)
{
	char format[3];
	int max_val;
	int result = 0;
	scanf("%s", format);
	if (strcmp(format, "P3") != 0) {
		printf("Eroare: trebuie sa fie P3\n");
		*exit = 1;
		return NULL;
	}
	scanf("%d %d %d", w, h, &max_val);
	m = malloc(*h * sizeof(pixel *));
	if (!m) {
		*exit = 1;
		return NULL;
	}
	for (int i = 0; i < *h; i++) {
		m[i] = malloc(*w * sizeof(pixel));
		if (!m[i]) {
			free_image(m, i);
			*exit = 1;
			return NULL;
		}
	}
	for (int i = 0; i < *h; i++) {
		for (int j = 0; j < *w; j++) {
			int r, g, b;
			result += scanf("%d %d %d", &r, &g, &b);
			if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
				printf("Eroare: valoare pixel necorespunzatoare\n");
				free_image(m, *h);
				*exit = 1;
				return NULL;
			}
			m[i][j].r = r;
			m[i][j].g = g;
			m[i][j].b = b;
			m[i][j].l = (r + g + b) / 3;
		}
	}
	if (result != 3 * (*w) * (*h)) {//verifficare citire
		printf("Eroare: eroare citire pixeli\n");
		free_image(m, *h);
		*exit = 1;
		return NULL;
	}
	printf("Imagine citita cu succes [%d-%d]\n", *h, *w);
	return m;
}

void write(pixel **m, int w, int h)
{
	if (!m)
		return;
	printf("P3\n");
	printf("%d %d\n", w, h);
	printf("255\n"); //maxvalue mereu 255
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			printf("%d %d %d\n", m[i][j].r, m[i][j].g, m[i][j].b);
		}
	}
}

pixel **resize(pixel **m, int *w, int *h)
{
	int new_w = (*w) * 4;//noile dimensiuni
	int new_h = (*h) * 4;

	pixel **new_m = malloc(new_h * sizeof(pixel *));
	if (!new_m) {
		return m;
	}

	for (int i = 0; i < new_h; i++) {
		new_m[i] = malloc(new_w * sizeof(pixel));
		if (!new_m[i]) {
			free_image(new_m, new_h);
			return NULL;
		}
	}
	for (int i = 0; i < new_h; i++) {
		for (int j = 0; j < new_w; j++) {
			int old_i = i / 4;
			int old_j = j / 4;
			new_m[i][j] = m[old_i][old_j];
		}
	}
	free_image(m, *h);
	*w = new_w;
	*h = new_h;
	printf("Imagine redimensionata cu succes [%d-%d]\n", new_h, new_w);
	return new_m;
}

void free_image(pixel **m, int h)
{
	if (m) {
		for (int i = 0; i < h; i++) {
			if (m[i]) {
				free(m[i]);
			}
		}
		free(m);
	}
}

int **create_grid(pixel **m, int w, int h)
{

	int grid_w = (w / 4) + 1;
	int grid_h = (h / 4) + 1;
	int **grid = calloc(grid_h, sizeof(int *));
	for (int i = 0; i < grid_h; i++) {
		grid[i] = calloc(grid_w, sizeof(int));
	}
	for (int i = 0; i < grid_h; i++) {
		for (int j = 0; j < grid_w; j++) {
			int sum = 0;
			int count = 0;
			// pixelul relevant
			int pi = i * 4;
			int pj = j * 4;
			if (pi < h && pj < w) {//cazuri normale
				sum += m[pi][pj].l;
				count++;
			} //exceptii
			if (pi >= 1 && pj >= 1) {
				sum += m[pi - 1][pj - 1].l;
				count++;
			}
			if (pi + 1 < h && pj + 1 < w) {
				sum += m[pi + 1][pj + 1].l;
				count++;
			}
			if (pj >= 1 && pi + 1 < h) {
				sum += m[pi + 1][pj - 1].l;
				count++;
			}
			if (pj + 1 < w && pi >= 1) {
				sum += m[pi - 1][pj + 1].l;
				count++;
			}
			grid[i][j] = sum / count;
			if (grid[i][j] <= 200)
				grid[i][j] = 1;
			else
				grid[i][j] = 0;
		}
	}
	return grid;
}

void march(pixel **m, int w, int h)
{
	int contur[16][16];
	//int matrice[5][5];
	init_contur(contur);
	int **grid = create_grid(m, w, h);
	int val = 0;
	for (int i = 0; i < h / 4 ; i++) {
		for (int j = 0; j < w / 4; j++) {
			val = grid[i][j] * 8 + grid[i][j + 1] * 4;
			val += grid[i + 1][j + 1] * 2 + grid[i + 1][j];
			for (int t = 4 * i; t < 4 * i + 4; t++)//transformare
				for (int k = 4 * j; k < 4 * j + 4; k++) {
					//linia e val si coloana calculam dupa formula
					m[t][k].l = contur[val][4 * (t % 4) + k % 4];
					m[t][k].r = m[t][k].l;
					m[t][k].g = m[t][k].l;
					m[t][k].b = m[t][k].l;
				}
		}
	}
	for (int i = 0; i < h / 4 + 1; i++) {
		free(grid[i]);
	}
	free(grid);
	printf("Marching Squares aplicat cu succes [%d-%d]\n", h, w);
}

void init_contur(int contur[16][16])
{
	int valori[16][16] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 0, 180, 255, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 255, 180},
	{0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 180, 180, 180, 180},
	{0, 0, 255, 180, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 255, 180, 0, 0, 0, 255, 255, 0, 0, 0, 180, 255, 0, 0},
	{0, 0, 255, 180, 0, 0, 255, 180, 0, 0, 255, 180, 0, 0, 255, 180},
	{0, 255, 180, 180, 255, 180, 180, 180, 180, 180, 180,
	 180, 180, 180, 180, 180},
	{180, 255, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{180, 255, 0, 0, 180, 255, 0, 0, 180, 255, 0, 0, 180, 255, 0, 0},
	{180, 255, 0, 0, 255, 0, 0, 0, 0, 0, 0, 255, 0, 0, 255, 180},
	{180, 180, 255, 0, 180, 180, 180, 255, 180, 180, 180, 180, 180,
	 180, 180, 180},
	{180, 180, 180, 180, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0},
	{180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 255, 180,
	 180, 255, 0},
	{180, 180, 180, 180, 180, 180, 180, 180, 255, 180, 180, 180, 0,
	 255, 180, 180},
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	 255, 255, 255}
	};
	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 16; j++)
			contur[i][j] = valori[i][j];
}

