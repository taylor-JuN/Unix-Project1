#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/times.h>

// 사용된 함수들
double start();
void getRecipe();
void modRecipe();
int getScore();

// 몇 번 맞추었는지 판단하는 전역 변수
int score = 0;

int main(void)
{

    printf("-------레시피 맞추기 게임-------\n");
    //무슨 명령을 따를지 입력 받는 변수
    int command = 0;

    while (command != 5)
    {
        printf("1.게임 시작 2.레시피 보기 3. 나의 점수 보기 4. 레시피 수정 5. 끝내기 \n");
        scanf("%d", &command);
        if (command == 1)
        {
            // start()함수의 return 값을 시간으로 주워서 한 문제 푸는데 몇 분 걸리는지 체크
            double time = start();
            printf("%.2f 만큼 걸렸습니다!\n", time);
        }
        else if (command == 2)
        {
            getRecipe();
        }
        else if (command == 3)
        {
            int x = getScore();
            printf("%d 번 맞추셨습니다!! \n", x);
        }
        else if (command == 4)
        {
            modRecipe();
        }
        else
        {
            exit(1);
        }
    }
}

// 게임을 시작하는 함수
double start()
{
    // fork() 및 반환 값을 위한 변수들
    int pid;
    int status;
    // 메뉴값
    char menu[1024];

    // 시간체크를 위한 변수 및 구조체 선언
    time_t t;
    struct tms mytms;
    clock_t t1, t2;
    clock_t CT = sysconf(_SC_CLK_TCK);

    // 메뉴 입력받음
    printf("만들 메뉴를 선택해주세요!\n");
    printf("메뉴의 종류 입니다.\n");
    system("ls /Users/taylor/prog/code/unix/project/quest/");
    scanf("%s", menu);

    // fork()를 통해 새로운 자식 프로세스 생성(이유는 한 문제 푸는데 걸린 시간을 알기 위하여)
    pid = fork();
    if (pid > 0) // 부모 프로세스, 자식프로세스를 wait하며 기다린 시간을 잰다.
    {
        //시작 시간을 저장
        if ((t1 = times(&mytms)) == -1)
        {
            perror("times 1");
            exit(1);
        }

        //status 반환값으로 자식이 프로세스를 종료하는지를 판단
        while (wait(&status) != pid)
        {
        };
        // 끝난 시간을 저장
        if ((t2 = times(&mytms)) == -1)
        {
            perror("times 2");
            exit(1);
        }

        printf("부모프로세스 실행 완료\n"); // 부모 프로세스가 끝났음을 알림
        // 8비트 이동하여, 반환값이 10이 아니면 score를 올려서 총 몇 문제를 맞혔는지를 계산
        if ((status >> 8) != 10)
        {
            score++;
        }

        return (double)(t2 - t1) / CT; // 한 문제 푸는데 총 몇 초 걸렸는지를 체크
    }

    // 자식프로세스. 실질적인 게임의 시작
    else if (pid == 0)
    {
        // 아규먼트들을 초기화해줌, menu는 부모 프로세스에서 받은 것을 그대로 사용
        char *argv[3];
        argv[0] = "game";
        argv[1] = menu;
        argv[2] = NULL;

        int x = 0;

        //execv()함수를 통해 다른 프로세스를 실행 시킨다. game 함수에서 추가 설명
        if ((x = execv("/Users/taylor/prog/code/unix/project/game", argv)) == -1)
        {
            perror("error!!");
        }

        // game프로세스가 잘 실행되었으면 실행되지 않는 printf문이다.
        printf("자식 프로세스 실행중\n");

        exit(2);
    }
    else
    {
        // fork()가 잘못되었을 때 에러나는 부분
        perror("fork error\n");
        exit(3);
    }
}

// 총 몇번 맞췄는지 알려주는 score를 반환하는 함수
int getScore()
{
    return score;
}

// 레시피를 알려주는 함수
void getRecipe()
{
    //절대경로를 ./recipe로 지정해줌.
    char path[1024] = "/Users/taylor/prog/code/unix/project/recipe/";
    char menu[1024];
    char buf[1024];
    printf("메뉴를 입력하세요\n");

    //먼저 레시피 폴더에 무엇이 있는지를 명령어로 보여줌.
    system("ls /Users/taylor/prog/code/unix/project/recipe/");
    scanf("%s", menu);
    int c;

    strcat(path, menu);

    FILE *fp;
    //파일포인터로 염, 읽기권한으로 염, 에러시 프린트문 출력
    if ((fp = fopen(path, "r")) == NULL)
    {
        printf("메뉴판에 없는 메뉴입니다. \n");
    }
    //바로 위에서 연 파일을 한 문자씩 갖고와 출력해줌. 결국 파일을 읽어 출력해줌
    while ((c = fgetc(fp)) != EOF)
    {
        printf("%c", c);
    }
}

//레시피를 수정하거나 추가할 수 있는 함수
void modRecipe()
{

    //절대경로 및 입력값을 받을 변수 선언
    char path[1024] = "/Users/taylor/prog/code/unix/project/recipe/";
    char path_q[1024] = "/Users/taylor/prog/code/unix/project/quest/";
    char menu[1024];
    char recipe_buf[1024];
    int cmd = 0;
    char buf[BUFSIZ][BUFSIZ];
    char del[BUFSIZ];
    char *arr[BUFSIZ];
    char *temp;
    int i = 0;
    int x = 0;

    printf("1.추가 2.삭제 \n");
    scanf("%d", &cmd);
    FILE *wfp, *wfp_q, *fp;
    int c = 0;
    if (cmd == 1)
    {
        printf("추가할 메뉴를 입력하세요\n");
        scanf("%s", menu);
        // 읽기 권한이기에 없으면 파일을 생성
        if (((wfp = fopen(strcat(path, menu), "w+")) == NULL))
        {
            perror("cannot open file");
            exit(1);
        }
        // exit를 받기 전까지 입력을 계속 받아 새로운 파일에 입력받은 값을 파일출력함, "exit"입력시 파일에 출력되지 않고 while문 탈출
        while (strcmp(recipe_buf, "exit\n"))
        {
            fgets(recipe_buf, 1024, stdin);

            if (strcmp(recipe_buf, "exit\n"))
            {
                fputs(recipe_buf, wfp);
            }
        }

        fclose(wfp);

        //이후 문제를 만들기 위해 다시 파일을 오픈한다.
        if (((fp = fopen(path, "r")) == NULL))
        {
            perror("cannot open file ");
            exit(1);
        }
        //레시피를 한 문장씩 가져와 arr배열에 저장한다.
        fgets(del, BUFSIZ, fp);
        while (fgets(buf[i], BUFSIZ, fp) != NULL)
        {
            arr[i] = buf[i];
            i++;
        }
        //quest폴더에(문제 파일) 파일을 생성한다.
        if (((wfp_q = fopen(strcat(path_q, menu), "w")) == NULL))
        {
            perror("cannot open file");
            exit(1);
        }

        // 타임 라이브리를 사용하여 래시피의 순서를 임의적으로 바꾼다.
        int q[i];
        srand(time(NULL));
        int rand_idx;
        for (int j = 0; j < i - 1; j++)
        {
            rand_idx = rand() % (i - j) + j;
            temp = arr[j];
            arr[j] = arr[rand_idx];
            arr[rand_idx] = temp;
        }

        // 원래의 순서와 비교하여 quest맨 위에 있는 답을 q배열에 저장한다.
        for (int z = 0; z < i; z++)
        {
            for (int s = 0; s < i; s++)
            {
                if (!strcmp(buf[z], arr[s]))
                {
                    q[s] = z + '0' + 1;
                }
            }
        }

        // quest파일에 저장한다.
        for (int c = 0; c < i; c++)
        {
            fputc(q[c], wfp_q);
        }
        fputc('\n', wfp_q);

        while (arr[x] != NULL)
        {
            fputs(arr[x], wfp_q);
            x++;
        }
    }
    // 레시피 삭제
    else if (cmd == 2)
    {
        printf("삭제할 메뉴를 입력하세요\n");
        scanf("%s", menu);
        //명령어를 사용하기 위해 strcat사용
        char x[1024] = "rm -f ./recipe/";
        char y[1024] = "rm -f ./quest/";
        strcat(x, menu);
        strcat(y, menu);
        system(x);
        system(y);
    }
    else
    {
        printf("잘못 입력하셨습니다.");
        perror("Not a Command");
        exit(1);
    }
    fclose(fp);
    fclose(wfp);
    fclose(wfp_q);
}
