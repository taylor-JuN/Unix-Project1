#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    //각종 변수들 선언
    FILE *fp;
    char path[1024] = "/Users/taylor/prog/code/unix/project/quest/";

    int answer[10];
    char x[10];
    int i = 0;
    int c = 0;
    int j = 0;
    int count = 0;        // 레시피 순서 맞췄는지를 판단하는 변수
    char *menu = argv[1]; // 프로세스를 생성할때 (execv()함수를 자식프로세스에서 불름) menu입력값으로 인자로 받아옴

    // 인자값과 절대경로를 합쳐 파일을 여는데 사용
    strcat(path, menu);

    if ((fp = fopen(path, "r")) == NULL)
    {
        perror("cannot open file");
        exit(10);
    }
    // quest의 파일 맨 윗줄에는 답이 적혀있음, 한글자씩 가져와 x배열에 하나씩 저장
    while ((c = fgetc(fp)) != '\n')
    {
        x[i] = c;
        i++;
    }

    // 섞인 레시피의 순서를 보여줌
    while ((c = fgetc(fp)) != EOF)
    {
        printf("%c", c);
    }
    printf("\n");

    while (x[j] > 0)
    {
        printf("답을한글자씩 입력하세요 \n");
        scanf("%d", &answer[j]);

        //입력 받은 답과 같은지를 판단, 맞으면 count를 증가, 틀리면 아무것도 하지 않음
        if ((x[j] - '0') == (answer[j]))
        {
            printf("맞았습니다!\n");
            count++;
        }
        else
        {
            printf("틀렸습니다! \n");
        }
        j++;
    }

    // count가 x배열의 길이(총 몇번의 레시피가 있는지)가 같으면 모두 맞음, exit(1)로 부모프로세스의 wait상태에 영향을 줌( 모두 맞췄을 경우 전역 변수인 proj.c의 count를 증가시켜 총 몇 번 맞췄는지를 알려줌)
    if (count == i)
    {
        printf("정답입니다! \n");
        exit(1);
    }
    else
    {
        printf("오답입니다! \n");
        exit(10);
    }
}
