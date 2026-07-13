#include <stdio.h>
#include <string.h>

int main()
{
    int T;
    scanf("%d", &T);

    while(T--)
    {
        char str[1001];
        scanf("%s", str);

        int sum = 0;

        for(int i = 0; str[i] != '\0'; i++)
        {
            if(str[i] >= '0' && str[i] <= '9')
            {
                sum += str[i] - '0';
            }
        }

        printf("%d\n", sum);
    }

    return 0;
}