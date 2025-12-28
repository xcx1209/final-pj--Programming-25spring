#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define MAX_ROWS 8
#define MAX_COLS 16

//游戏开始选择函数
void choose() {
	printf("#####################################################################\n");
	printf("##                            欢迎！                               ##\n");
	printf("##                         推箱子游戏                              ##\n");
	printf("#####################################################################\n\n");
	printf("是否开始游戏（输入“y”表示“是”，输入“n”表示“否”):");
	char choice[2];
		fgets(choice, 2, stdin);
		char c;
	while ((c = getchar()) != '\n' && c != EOF);//清理缓冲区\n，避免main函数中直接读取"\n"导致直接退出循环
	if (choice[0] == 'n') {
	printf("游戏结束。\n");
		exit(1);
	}
	else if (choice[0] == 'y') {
		printf("*********************************\n");
		printf("*******     游戏开始！     ******\n");
		printf("*******    #：    墙壁     ******\n");
		printf("*******    $：    箱子     ******\n");
		printf("*******    .：    目标点   ******\n");
		printf("*******    @：    玩家     ******\n");
		printf("*******   空格：  空地     ******\n");
		printf("*********************************\n");

		return;
	}
	else {
		printf("输入错误，请重新输入：");
	}

}

struct position {
	int i;
	int j;
	int value;
};//三个信息i,j,map[i][j]

struct role {
	struct position pos[3];//
};
//定义栈
struct role stack[100];
int top = -1;//栈顶标记-1

//入栈操作
void push(struct position array[]) {
	top++;
	stack[top].pos[0] = array[0];
	stack[top].pos[1] = array[1];
	stack[top].pos[2] = array[2];
}

int map[MAX_ROWS][MAX_COLS];
FILE* file;//地图文件
FILE* file1;//体力文件
FILE* fp;//地图存档
FILE* fp1;//体力存档

void game(int map_number) {

	int count = 0;//体力值初始化
	// 选择文件
	if (map_number == 1) {
		file = fopen("map1.txt", "r");
	}
	if (map_number == 2) {
		file = fopen("map2.txt", "r");
		printf("限制体力为：60\n");
	}
	if (map_number == 3) {
		file = fopen("record.txt", "r");
		file1 = fopen("count.txt", "r");
		fscanf(file1, "%d", &count);
	}

	int rows, cols;
	fscanf(file, "%d %d", &rows, &cols);


	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			fscanf(file, "%d", &map[i][j]);
		}
	}
	fclose(file);
	
	while (1) {
		for (int i = 0;i < rows;i++) {
			for (int j = 0;j < cols;j++) {
				switch (map[i][j]) {
				case 0: printf(" "); break;
				case 1: printf("#"); break;
				case 2: printf("$"); break;
				case 3: printf("."); break;
				case 4: printf("@"); break;
				case 5: printf("*"); break;
				}
			}
			printf("\n");
		}
		//找玩家@(4)在哪里
		int i, j;
		for (i = 0;i < rows;i++) {
			for (j = 0;j < cols;j++) {
				if (map[i][j] == 4) {
					break;
				}
			}
			if (map[i][j] == 4) {
				break;
			}
		}
		//i和j表示人的坐标

		//移动操作
		/*  w：上；
			s：下；
			a：左；
			d：右
			只有空地0，箱子2可以前进	*/
		printf("\n");
		printf("输入p，游戏存档。\n");
		printf("退出游戏，请按k。\n");
		printf("请输入移动操作（上w，下s，左a，右d）：\n");
		printf("撤回上一步操作，请按z\n");
		printf("——————————————————————————————\n");
		printf("已用体力：%d\n", count);

		struct position pos[3];

		char key[2];
		fgets(key, 2, stdin);
		char c;
		char route[100];

		while ((c = getchar()) != '\n' && c != EOF); //清除缓冲，避免循环读取"\n"运行default
			switch (key[0]) {
			case('w')://向上
			case('W'):
				pos[0].i = i;
				pos[0].j = j;
				pos[0].value = map[i][j];

				pos[1].i = i - 1;
				pos[1].j = j;
				pos[1].value = map[i - 1][j];

				pos[2].i = i - 2;
				pos[2].j = j;
				pos[2].value = map[i - 2][j];

				push(pos);

				if (map[i - 1][j] == 0) { //人上方是空地
					map[i][j] -= 4;
					map[i - 1][j] += 4;
					count++;
				}
				if (map[i - 1][j] == 2 && map[i - 2][j] == 0) { //人上方是箱子，箱子上方是空地
					map[i][j] -= 4;
					map[i - 1][j] -= 2;
					map[i - 1][j] += 4;
					map[i - 2][j] += 2;
					count++;
				}
				if (map[i - 1][j] == 1 || (map[i - 1][j] == 2 && map[i - 2][j] == 1)) { //人上方是墙或人上方是箱子，箱子上方是墙
					printf("\n");
					printf("！！！！！！！！！！！！！！！\n");
					printf("！   前面是墙，不能前进！   ！\n");
					printf("！！！！！！！！！！！！！！！\n");
					printf("\n");
				}
				if (map[i - 1][j] == 2 && map[i - 2][j] == 3) { //人上方是箱子，箱子上方是目标点
					map[i][j] -= 4;
					map[i - 1][j] -= 2;
					map[i - 1][j] += 4;
					map[i - 2][j] -= 3;
					map[i - 2][j] += 5;
					count++;
				}
				route[count - 1] = 'W';
				break;

			case('s')://向下
			case('S'):

				pos[0].i = i;
				pos[0].j = j;
				pos[0].value = map[i][j];

				pos[1].i = i + 1;
				pos[1].j = j;
				pos[1].value = map[i + 1][j];

				pos[2].i = i + 2;
				pos[2].j = j;
				pos[2].value = map[i + 2][j];

				push(pos);

				if (map[i + 1][j] == 0) { //人下方是空地
					map[i][j] -= 4;
					map[i + 1][j] += 4;
					count++;
				}
				if (map[i + 1][j] == 2 && map[i + 2][j] == 0) { //人下方是箱子，箱子下方是空地
					map[i][j] -= 4;
					map[i + 1][j] -= 2;
					map[i + 1][j] += 4;
					map[i + 2][j] += 2;
					count++;
				}
				if (map[i + 1][j] == 1 || (map[i + 1][j] == 2 && map[i + 2][j] == 1)) { //人下方是墙或人下方是箱子，箱子下方是墙
					printf("\n");
					printf("！！！！！！！！！！！！！！！\n");
					printf("！   前面是墙，不能前进！   ！\n");
					printf("！！！！！！！！！！！！！！！\n");
					printf("\n");
				}
				if (map[i + 1][j] == 2 && map[i + 2][j] == 3) { //人下方是箱子，箱子下方是目标点
					map[i][j] -= 4;
					map[i + 1][j] -= 2;
					map[i + 1][j] += 4;
					map[i + 2][j] -= 3;
					map[i + 2][j] += 5;
					count++;
				}
				route[count - 1] = 'S';
				break;

			case('a')://向左
			case('A'):

				pos[0].i = i;
				pos[0].j = j;
				pos[0].value = map[i][j];

				pos[1].i = i;
				pos[1].j = j - 1;
				pos[1].value = map[i][j - 1];

				pos[2].i = i;
				pos[2].j = j - 2;
				pos[2].value = map[i][j - 2];

				push(pos);

				if (map[i][j - 1] == 0) { //人左边是空地
					map[i][j] -= 4;
					map[i][j - 1] += 4;
					count++;
				}
				if (map[i][j - 1] == 2 && map[i][j - 2] == 0) { //人左边是箱子，箱子左边是空地
					map[i][j] -= 4;
					map[i][j - 1] -= 2;
					map[i][j - 1] += 4;
					map[i][j - 2] += 2;
					count++;
				}
				if (map[i][j - 1] == 1 || (map[i][j - 1] == 2 && map[i][j - 2] == 1)) { //人左边是墙或人左边是箱子，箱子左边是墙
					printf("\n");
					printf("！！！！！！！！！！！！！！！\n");
					printf("！   前面是墙，不能前进！   ！\n");
					printf("！！！！！！！！！！！！！！！\n");
					printf("\n");
				}
				if (map[i][j - 1] == 2 && map[i][j - 2] == 3) { //人左边是箱子，箱子左边是目标点
					map[i][j] -= 4;
					map[i][j - 1] -= 2;
					map[i][j - 1] += 4;
					map[i][j - 2] -= 3;
					map[i][j - 2] += 5;
					count++;
				}
				route[count - 1] = 'A';
				break;

			case('d')://向右
			case('D'):

				pos[0].i = i;
				pos[0].j = j;
				pos[0].value = map[i][j];

				pos[1].i = i;
				pos[1].j = j + 1;
				pos[1].value = map[i][j + 1];

				pos[2].i = i;
				pos[2].j = j + 2;
				pos[2].value = map[i][j + 2];

				push(pos);

				if (map[i][j + 1] == 0) { //人右边是空地
					map[i][j] -= 4;
					map[i][j + 1] += 4;
					count++;
				}
				if (map[i][j + 1] == 2 && map[i][j + 2] == 0) { //人右边是箱子，箱子右边是空地
					map[i][j] -= 4;
					map[i][j + 1] -= 2;
					map[i][j + 1] += 4;
					map[i][j + 2] += 2;
					count++;
				}
				if (map[i][j + 1] == 1 || (map[i][j + 1] == 2 && map[i][j + 2] == 1)) { //人右边是墙或人右边是箱子，箱子右边是墙
					printf("\n");
					printf("！！！！！！！！！！！！！！！\n");
					printf("！   前面是墙，不能前进！   ！\n");
					printf("！！！！！！！！！！！！！！！\n");
					printf("\n");
				}
				if (map[i][j + 1] == 2 && map[i][j + 2] == 3) { //人左边是箱子，箱子左边是目标点
					map[i][j] -= 4;
					map[i][j + 1] -= 2;
					map[i][j + 1] += 4;
					map[i][j + 2] -= 3;
					map[i][j + 2] += 5;
					count++;
				}
				route[count - 1] = 'D';
				break;

			case('p')://游戏存档
			case('P'):
				//记录地图
				fp = fopen("record.txt", "w+");
				fprintf(fp, "%d %d\n", rows, cols);

				for (int i = 0;i < rows;i++) {
					for (int j = 0;j < cols;j++) {
						fprintf(fp, "%d ", map[i][j]);
					}
					fprintf(fp, "\n");
				}
				fclose(fp);
				//记录体力
				fp1 = fopen("count.txt", "w+");

				fprintf(fp1, "%d", count);

				fclose(fp1);
				break;

			case ('k')://退出游戏
			case ('K'):
				return 0;

			case ('z')://撤回
			case ('Z'):
				if (top != -1) {
					map[stack[top].pos[0].i][stack[top].pos[0].j] = stack[top].pos[0].value;
					map[stack[top].pos[1].i][stack[top].pos[1].j] = stack[top].pos[1].value;
					map[stack[top].pos[2].i][stack[top].pos[2].j] = stack[top].pos[2].value;
					top--;
					count--;
				}
				break;

			default:
				printf("=======================================\n");
				printf("===  输入错误指令，请重新输入！！！ ===\n");
				printf("=======================================\n");
				break;
			}

			//体力限制
			if (count > 60) {
				printf("#######################");
				printf("# 体力耗尽，游戏失败！#");
				printf("#######################");
				return 0;
			}

			//查找是否所有箱子都到达目标点
			int find = 0;
			for (i = 0;i < rows;i++) {
				for (j = 0;j < cols;j++) {
					if (map[i][j] != 3) {//地图中已无目标点
						find++;
					}
				}
			}
			if (find == rows * cols) {
				printf("*********************************\n");
				printf("***   恭喜你，关卡挑战成功！  ***\n");
				printf("***       消耗体力为：%d      ***\n", count);
				printf("路径为：");
				for (int i = 0;i < count;i++) {
					printf("%c ", route[i]);
				}
				printf("\n");
				printf("*********************************\n");
				break;

			}
		}
	fclose(file);
}


int main() {
	choose();
	printf("——————————————————————————————\n");
	printf("请选择关卡（第一关输入1；第二关输入2）：\n");
	printf("* 第一关（输入1） *\n");
	printf("* 第二关（输入2） *\n");
	printf("读取存档文件（输入3）：\n");
	printf("——————————————————————————————\n");
	char game_level[2];
	fgets(game_level, 2, stdin);
	char c;
	while ((c = getchar()) != '\n' && c != EOF);//清理缓冲区\n，避免MAP1函数中直接读取"\n"导致第一个操作无法正确执行
	if (game_level[0] == '1') {
		game(1);//选择1，则执行关卡1，map1.txt
	}
	if (game_level[0] == '2') {
		game(2);//选择2，则执行关卡2，map2.txt
	}
	if (game_level[0] == '3') {
		game(3);//选择3，则读取游戏存档文件record.txt,恢复存档状态
	}

	return 0;
}
